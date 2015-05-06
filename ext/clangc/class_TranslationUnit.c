/*TranslationUnit ruby class*/
#include "TranslationUnit.h"
static void
c_TranslationUnit_struct_free(TranslationUnit_t *s)
{
  if(s)
  {
    
  if(s->data)
    clang_disposeTranslationUnit(s->data); 

    ruby_xfree(s);
  }
}  
static VALUE
c_TranslationUnit_struct_alloc( VALUE klass)
{
  
    TranslationUnit_t * ptr;
    ptr = (TranslationUnit_t *) ruby_xmalloc(sizeof(TranslationUnit_t)); 
    ptr->data = NULL;

  return Data_Wrap_Struct(klass, NULL, c_TranslationUnit_struct_free, (void *) ptr);
}

static VALUE
c_TranslationUnit_initialize(VALUE self) {
  
}
VALUE
generate_TranslationUnit_under(VALUE module, VALUE superclass)
{
  VALUE klass = rb_define_class_under(module, "TranslationUnit", superclass);
  rb_define_alloc_func(klass, c_TranslationUnit_struct_alloc);
  return klass;
}

