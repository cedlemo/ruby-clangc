/*Index ruby class*/
#include "clang-c/Index.h"
#include "class_Index.h"
#include "stdio.h"

static void
c_Index_struct_free(Index_t *s)
{
  if(s)
  {
    
    if(s->data)
    {
      clang_disposeIndex(s->data); 
    }
    ruby_xfree(s);
   }
}  
static VALUE
c_Index_struct_alloc( VALUE klass)
{
  Index_t *i;
  i = (Index_t *) ruby_xmalloc(sizeof(Index_t));
  i->data = NULL;
  return Data_Wrap_Struct(klass, NULL, c_Index_struct_free,(void *) i );
}

static VALUE
c_Index_initialize(VALUE self, VALUE excl_decls_from_PCH, VALUE display_diagnostics) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  uint e= (excl_decls_from_PCH == Qtrue) ? 1 : 0;
  uint d= (display_diagnostics == Qtrue) ? 1 : 0;

  i->data = clang_createIndex( e, d);
  return self;
}
static VALUE
c_Index_set_global_options(VALUE self, VALUE options) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  if (TYPE(options) == T_FIXNUM || TYPE(options) == T_BIGNUM)
    clang_CXIndex_setGlobalOptions(i->data,NUM2UINT(options));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
  return Qnil;
}
static VALUE
c_Index_get_global_options(VALUE self) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);

  return UINT2NUM(clang_CXIndex_getGlobalOptions(i->data));
}
static VALUE
c_Index_create_TU_from_source_file(VALUE self, VALUE source_file, VALUE args) {
  VALUE tu;
  Check_Type(source_file, T_STRING); //Manage nil and String value
  Check_Type(args, T_ARRAY);
  int len = RARRAY_LEN(args);
  char *c_args[len];
  int i = 0;
  for (i=0; i< len; i++) {
    VALUE arg = rb_ary_entry(args, i);
    args[i] = stringValueCStr(arg);
  }
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  
  clang_createTranslationUnitFromSourceFile(i->data, "IndexTest.c", 2, args, 0, 0); // TODO manage unsaved files
  return tu;
}
VALUE
generate_Index_under(VALUE module, VALUE superclass) {
  VALUE klass = rb_define_class_under(module, "Index", superclass);
  rb_define_alloc_func(klass, c_Index_struct_alloc);
  rb_define_private_method(klass, "initialize", RUBY_METHOD_FUNC(c_Index_initialize), 2);
  rb_define_method(klass, "global_options=", RUBY_METHOD_FUNC(c_Index_set_global_options), 1);
  rb_define_method(klass, "global_options", RUBY_METHOD_FUNC(c_Index_get_global_options), 0);
  return klass;
}

