#ifndef MACROS_H
#define MACROS_H
/*****************/
/*Debugging tools*/
/*****************/
#include <stdio.h>
#include <errno.h>
#include <string.h>

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
#define RBOOL_2_INT(arg, c_arg)\
c_arg = (arg == Qtrue) ? 1 : 0;
//ruby number to uint
#define RNUM_2_UINT(rval, cval)\
if (TYPE(options) == T_FIXNUM || TYPE(options) == T_BIGNUM)\
  cval=NUM2UINT(options);\
else\
  rb_raise(rb_eTypeError, "invalid type for input");

/****************/
/*Classes Macros*/
/****************/
//Create a new instance without args and get its data
#define R_GET_CLASS_DATA(module_name, class_name, instance, data_type, data_ptr)\
VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));\
  VALUE cKlass = rb_const_get(mModule, rb_intern(class_name));\
  instance = rb_class_new_instance(0, NULL, cKlass);\
  Data_Get_Struct(tu, data_type, data_ptr);
//Create a new instance with args and get its data
#define R_GET_CLASS_WITH_ARGS_DATA(module_name, class_name, instance, args, data_type, data_ptr)\
VALUE mModule = rb_const_get(rb_cObject, rb_intern(module_name));\
  VALUE cKlass = rb_const_get(mModule, rb_intern(class_name));\
  int args_n=CARRAY_LEN(args)\
  instance = rb_class_new_instance(args_len, args, cKlass);\
  Data_Get_Struct(tu, data_type, data_ptr);


#endif //MACROS_H
