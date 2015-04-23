#include <ruby/ruby.h>
#include "clang-c/Index.h"

#include "constants.h"
void Init_clangc(void) {
  VALUE m_clangc = rb_define_module("clangc");
  init_clang_enums_to_constants(m_clangc);
  
}
