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
#define SENTINEL(M, ...) printf("[SENTINEL] (%s:%s:%d: errno: %s) " M "\n",__FILE__, __FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define LOG_ERR(M, ...) fprintf(stderr,  "[ERROR] (%s:%s:%d: errno: %s) " M "\n", __FILE__, __FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define LOG_WARN(M, ...) fprintf(stderr, "[WARN]  (%s:%s:%d: errno: %s) " M "\n", __FILE__, __FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define LOG_INFO(M, ...) fprintf(stderr, "[INFO]  (%s:%s:%d) " M "\n", __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }
/*****************/
/*Common C macros*/
/*****************/
#define CARRAY_LEN(A)   (sizeof(A) / sizeof((A)[0]));
#define CHECK_IN_RANGE(A, MIN, MAX)\
  if (A < MIN)\
    A = MIN;\
  if (A > MAX)\
    A = MAX;

/***************************/
/*Ruby arguments to C value*/
/***************************/
//ruby array of string to C array
#define RARRAY_OF_STRINGS_2_C(args)\
Check_Type(args, T_ARRAY);\
int len = RARRAY_LEN(args);\
const char * c_args[len];\
if(len > 0)\
{\
  int j = 0;\
  for (j=0; j< len; j++) {\
    VALUE arg = rb_ary_entry(args, j);\
    c_args[j] = StringValueCStr(arg);\
  }\
}
//ruby boolean value to C int
static inline int rbool_2_int(VALUE rval)
{
  return (int) ((rval == Qtrue)? 1 : 0);
}
#define RBOOL_2_INT(a) rbool_2_int(a)

//Ruby string to C char 
static inline char * rstring_2_char(VALUE rval)
{
  VALUE rb_string = StringValue(rval);
  return StringValueCStr(rval);
}
#define RSTRING_2_CHAR(rval) rstring_2_char(rval)

/****************/
/*Classes Macros*/
/****************/
//Create a new instance without args and get its data
#define R_GET_CLASS_DATA(module_name, class_name, instance, data_type, data_ptr)\
VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));\
  VALUE cKlass = rb_const_get(mModule, rb_intern(class_name));\
  instance = rb_class_new_instance(0, NULL, cKlass);\
  Data_Get_Struct(instance, data_type, data_ptr);

#define R_GET_CLASS_DATA1(module_name, class_name, instance, data_ptr)\
VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));\
  VALUE cKlass = rb_const_get(mModule, rb_intern(#class_name));\
  instance = rb_class_new_instance(0, NULL, cKlass);\
  Data_Get_Struct(instance, class_name ##_t, data_ptr);

//Create a new instance with args and get its data
#define R_GET_CLASS_WITH_ARGS_DATA(module_name, class_name, instance, args, data_type, data_ptr)\
VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));\
  VALUE cKlass = rb_const_get(mModule, rb_intern(class_name));\
  int args_n=CARRAY_LEN(args)\
  instance = rb_class_new_instance(args_len, args, cKlass);\
  Data_Get_Struct(instance, data_type, data_ptr);

/************************/
/*C values to Ruby value*/
/************************/
static inline VALUE cxstring_2_rval(CXString str)
{
  VALUE r_string = rb_str_new2(clang_getCString(str));
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

#endif //MACROS_H
