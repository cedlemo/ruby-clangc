/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015  cedlemo <cedlemo@gmx.com>
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
#include <ruby/ruby.h>
#include "clang-c/Index.h"

#include "constants.h"
#include "class_Index.h"
void Init_clangc(void) {
  VALUE m_Clangc = rb_define_module("Clangc");
  init_clang_enums_to_constants(m_Clangc);

/*
* An "index" consists of a set of translation units that would
* typically be linked together into an executable or library.
*/
  VALUE c_Index = rb_define_class_under(m_Clangc, "Index", rb_cObject);
  rb_define_alloc_func(c_Index, c_Index_struct_alloc);
  rb_define_private_method(c_Index, "initialize", RUBY_METHOD_FUNC(c_Index_initialize), 2);// in class_Index.c
  rb_define_method(c_Index, "global_options=", RUBY_METHOD_FUNC(c_Index_set_global_options), 1);// in class_Index.c
  rb_define_method(c_Index, "global_options", RUBY_METHOD_FUNC(c_Index_get_global_options), 0);// in class_Index.c
  rb_define_method(c_Index, "create_translation_unit_from_source_file", RUBY_METHOD_FUNC(c_Index_create_TU_from_source_file), 2);// in class_Index.c
  rb_define_method(c_Index, "create_translation_unit", RUBY_METHOD_FUNC(c_Index_create_TU), 1);// in class_Index.c
  rb_define_method(c_Index, "parse_translation_unit", RUBY_METHOD_FUNC(c_Index_parse_TU), 3);// in class_Index.c

/*
* A single TranslationUnit which resides in an Index
*/
  VALUE c_TranslationUnit = rb_define_class_under(m_Clangc, "TranslationUnit", rb_cObject);
  rb_define_alloc_func(c_TranslationUnit, c_TranslationUnit_struct_alloc);
}
