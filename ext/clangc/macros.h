/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016 Cédric Le Moigne cedlemo <cedlemo@gmx.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MACROS_H
#define MACROS_H
/*****************/
/*Debugging tools*/
/*****************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ruby/ruby.h>
#include "clang-c/Index.h"

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define SENTINEL(M, ...)                                                       \
    printf("[SENTINEL] (%s:%s:%d: errno: %s) " M "\n",                         \
           __FILE__,                                                           \
           __FUNCTION__,                                                       \
           __LINE__,                                                           \
           clean_errno(),                                                      \
           ##__VA_ARGS__)
#define LOG_ERR(M, ...)                                                        \
    fprintf(stderr,                                                            \
            "[ERROR] (%s:%s:%d: errno: %s) " M "\n",                           \
            __FILE__,                                                          \
            __FUNCTION__,                                                      \
            __LINE__,                                                          \
            clean_errno(),                                                     \
            ##__VA_ARGS__)
#define LOG_WARN(M, ...)                                                       \
    fprintf(stderr,                                                            \
            "[WARN]  (%s:%s:%d: errno: %s) " M "\n",                           \
            __FILE__,                                                          \
            __FUNCTION__,                                                      \
            __LINE__,                                                          \
            clean_errno(),                                                     \
            ##__VA_ARGS__)
#define LOG_INFO(M, ...)                                                       \
    fprintf(stderr,                                                            \
            "[INFO]  (%s:%s:%d) " M "\n",                                      \
            __FILE__,                                                          \
            __FUNCTION__,                                                      \
            __LINE__,                                                          \
            ##__VA_ARGS__)
#define check(A, M, ...)                                                       \
    if (!(A))                                                                  \
    {                                                                          \
        log_err(M, ##__VA_ARGS__);                                             \
        errno = 0;                                                             \
    }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...)                                                 \
    if (!(A))                                                                  \
    {                                                                          \
        debug(M, ##__VA_ARGS__);                                               \
        errno = 0;                                                             \
        goto error;                                                            \
    }
/*****************/
/*Common C macros*/
/*****************/
#define CARRAY_LEN(A) (sizeof(A) / sizeof((A)[0]));
#define CHECK_IN_RANGE(A, MIN, MAX)                                            \
    if (A < MIN) A = MIN;                                                      \
    if (A > MAX) A = MAX;

/***************************/
/*Ruby arguments to C value*/
/***************************/
// ruby array of string to C array
#define RARRAY_OF_STRINGS_2_C(args)                                            \
    Check_Type(args, T_ARRAY);                                                 \
    int len = RARRAY_LEN(args);                                                \
    const char *c_args[len];                                                   \
    if (len > 0)                                                               \
    {                                                                          \
        int j = 0;                                                             \
        for (j = 0; j < len; j++)                                              \
        {                                                                      \
            VALUE arg = rb_ary_entry(args, j);                                 \
            c_args[j] = StringValueCStr(arg);                                  \
        }                                                                      \
    }
// ruby boolean value to C int
static inline int rbool_2_int(VALUE rval)
{
    return (int) ((rval == Qtrue) ? 1 : 0);
}
#define RBOOL_2_INT(a) rbool_2_int(a)

// Ruby string to C char
static inline char *rstring_2_char(VALUE rval)
{
    return StringValueCStr(rval);
}
#define RSTRING_2_CHAR(rval) rstring_2_char(rval)

static int symbol_2_const(char *module_name, char *enum_name, VALUE sym)
{
  VALUE main_module, enum_module, constant_name;
  main_module = rb_const_get(rb_cObject, rb_intern(module_name));
  enum_module = rb_const_get(main_module, rb_intern(enum_name));
  constant_name = rb_funcall(sym, rb_intern("to_s"), 0, NULL);
  constant_name = rb_funcall(constant_name, rb_intern("upcase"), 0, NULL);
  VALUE num =  rb_const_get(enum_module, rb_intern_str(constant_name));
  
  return NUM2UINT(num);
}

static int bitmask_or_array(char *module_name, char *enum_name, VALUE array)
{
  int or_sum = 0;
  int i = 0;
  int len = RARRAY_LEN(array);
  
  for(i = 0; i < len; i++)
  {
    VALUE sym = rb_ary_entry(array, i);
    or_sum = or_sum | symbol_2_const(module_name, enum_name, sym);
  }
  return or_sum;
}

static unsigned int rb_constant_argument_to_uint(char *module_name, char *enum_name, VALUE argument)
{
  if(TYPE(argument) == T_SYMBOL)
    return symbol_2_const(module_name, enum_name, argument);
  else if(TYPE(argument) == T_ARRAY)
    return bitmask_or_array(module_name, enum_name, argument);
  else
    return NUM2UINT(argument);
}
#define CLANGC_CONSTANT_TO_UINT(enum_name, argument) \
  rb_constant_argument_to_uint("Clangc", enum_name, argument)

/****************/
/*Classes Macros*/
/****************/
// Check argument
static inline void check_arg_type(VALUE arg, char *class_name)
{
    VALUE mModule;
    VALUE cKlass;
    char *message;
    int len;

    Check_Type(arg, T_DATA);
    mModule = rb_const_get(rb_cObject, rb_intern("Clangc"));
    cKlass = rb_const_get(mModule, rb_intern(class_name));
    
    len = asprintf(&message, "Clangc::%s Object was expected", class_name);
    
    if (cKlass != rb_funcall(arg, rb_intern("class"), 0))
    {
        if (len != -1)
        {
            rb_raise(rb_eTypeError, "%s", message);
            free(message);
        }
        else
            rb_raise(rb_eTypeError, "Not the object type expected");
    }
}

#define CHECK_ARG_TYPE(arg, classname) check_arg_type(arg, #classname)

// Create a new instance without args and get its data
#define R_GET_CLASS_DATA(module_name, class_name, instance, data_ptr)          \
    VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));          \
    VALUE cKlass = rb_const_get(mModule, rb_intern(#class_name));              \
    instance = rb_class_new_instance(0, NULL, cKlass);                         \
    Data_Get_Struct(instance, class_name##_t, data_ptr);

// Create a new instance with args and get its data
#define R_GET_CLASS_WITH_ARGS_DATA(                                            \
    module_name, class_name, instance, args, data_type, data_ptr)              \
    VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));          \
    VALUE cKlass = rb_const_get(mModule, rb_intern(class_name));               \
    int args_n = CARRAY_LEN(args) instance =                                   \
        rb_class_new_instance(args_len, args, cKlass);                         \
    Data_Get_Struct(instance, data_type, data_ptr);

/* For later.
#define NEW_CLASS_INSTANCE(class_name)\
static inline VALUE new_##class_name##_inst(class_name##_t *data)\
{\
  VALUE instance;\
  R_GET_CLASS_DATA("clangc", class_name, instance, data);\
  return instance;\
}
NEW_CLASS_INSTANCE(TranslationUnit)
#undef NEW_CLASS_INSTANCE*/

/************************/
/*C values to Ruby value*/
/************************/
static inline VALUE cxstring_2_rval(CXString str)
{
    VALUE r_string;
    const char *c_string =
        (clang_getCString(str) != NULL) ? clang_getCString(str) : "";
    r_string = rb_str_new2(c_string);
    clang_disposeString(str);
    return r_string;
}
#define CUINT_2_NUM(c_val) UINT2NUM(c_val)
#define CINT_2_NUM(c_val) INT2NUM(c_val)
#define CLLONG_2_NUM(c_val) LL2NUM(c_val)
#define CULLONG_2_NUM(c_val) ULL2NUM(c_val)
#define NOT_0_2_RVAL(c_val) ((c_val == 0) ? Qfalse : Qtrue)
#define EQ_1_2_RVAL(c_val) ((c_val == 1) ? Qtrue : Qfalse)
#define CXSTR_2_RVAL(str) (cxstring_2_rval(str))

#endif // MACROS_H
