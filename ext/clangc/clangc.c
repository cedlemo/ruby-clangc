#include <ruby/ruby.h>
#include "clang-c/Index.h"

#include "constants.h"
#include "class_Index.h"
void Init_clangc(void) {
  VALUE m_clangc = rb_define_module("Clangc");
  init_clang_enums_to_constants(m_clangc);
  generate_Index_under(m_clangc, rb_cObject);   
  generate_TranslationUnit_under(m_clangc, rb_cObject);   
}
