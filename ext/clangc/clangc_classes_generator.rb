#!/usr/bin/env ruby

require "./binder"

#index_class = Binder::Ruby_C_Class_Generator.new("Index", "CXIndex")
#index_class.free_instructions = %q{
#  if(s->data)
#    clang_disposeIndex(s->data); 
#}
#index_class.module_name("m_clangc")
#index_class.superclass_name("rb_cObject")
#index_class.generate_files

tu_class = Binder::Ruby_C_Class_Generator.new("TranslationUnit", "CXTranslationUnit")
tu_class.free_instructions = %q{
  if(s->data)
    clang_disposeTranslationUnit(s->data); 
}
tu_class.generate_files
