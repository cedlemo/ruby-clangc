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

#tu_class = Binder::Ruby_C_Class_Generator.new("TranslationUnit", "CXTranslationUnit", true)
#tu_class.free_instructions = %q{
#  if(s->data)
#    clang_disposeTranslationUnit(s->data); 
#}
#tu_class.generate_files

#diagnostic_class = Binder::Ruby_C_Class_Generator.new("Diagnostic", "CXDiagnostic", true)
#diagnostic_class.free_instructions = %q{
#  if(s->data)
#    clang_disposeDiagnostic(s->data); 
#}
#diagnostic_class.generate_files

#File_class = Binder::Ruby_C_Class_Generator.new("File", "CXFile", true)
#File_class.free_instructions not needed here
#File_class.generate_files
SourceRange_class = Binder::Ruby_C_Class_Generator.new("SourceRange", "CXSourceRange", true)
SourceRange_class.generate_files
