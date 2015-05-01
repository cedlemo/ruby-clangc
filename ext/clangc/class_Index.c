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
      printf("SENTINEL index ptr %p\n", s->data);
      clang_disposeIndex(s->data); 
    }
    printf("SENTINEL class ptr %p\n", s);
    ruby_xfree(s);
    printf("SENTINEL free end\n");
   }
}  
static VALUE
c_Index_struct_alloc( VALUE klass)
{
  Index_t *i;
  i = (Index_t *) ruby_xmalloc(sizeof(Index_t));
  printf("New class allocated at %p\n", i);
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
  printf("New class ptr %p with Index ptr %p\n", i, i->data);
  return self;
}
static VALUE
c_Index_set_global_options(VALUE self, VALUE options) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  if (TYPE(options) == T_FIXNUM || TYPE(options) == T_BIGNUM)
    clang_CXIndex_setGlobalOptions(i,NUM2UINT(options));
  else
    rb_raise(rb_eTypeError, "invalid type for input");
  return Qnil;
}
static VALUE
c_Index_get_global_options(VALUE self) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);

  return UINT2NUM(clang_CXIndex_getGlobalOptions(i));
}
VALUE
generate_Index_under(VALUE module, VALUE superclass){
  VALUE klass = rb_define_class_under(module, "Index", superclass);
  rb_define_alloc_func(klass, c_Index_struct_alloc);
  rb_define_private_method(klass, "initialize", RUBY_METHOD_FUNC(c_Index_initialize), 2);
  rb_define_method(klass, "global_options=", RUBY_METHOD_FUNC(c_Index_set_global_options), 1);
  rb_define_method(klass, "global_options", RUBY_METHOD_FUNC(c_Index_get_global_options), 0);
  return klass;
}

