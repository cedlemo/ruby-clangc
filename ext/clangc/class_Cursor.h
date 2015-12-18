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
#ifndef CURSOR_H
#define CURSOR_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct Cursor_t {
  CXCursor data; 
  VALUE parent;
} Cursor_t;

VALUE
c_Cursor_struct_alloc(VALUE);

VALUE
c_Cursor_is_null(VALUE);

VALUE
c_Cursor_is_equal(VALUE, VALUE);

VALUE
c_Cursor_get_hash(VALUE);

VALUE
c_Cursor_get_kind(VALUE);

VALUE
c_Cursor_get_linkage(VALUE);

VALUE
c_Cursor_get_availability(VALUE);

VALUE
c_Cursor_get_language(VALUE);

VALUE
c_Cursor_get_type(VALUE);

VALUE
c_Cursor_get_semantic_parent(VALUE);

VALUE
c_Cursor_get_lexical_parent(VALUE);

VALUE
c_Cursor_get_source_location(VALUE);

VALUE
c_Cursor_get_extent(VALUE);

VALUE
c_Cursor_get_spelling(VALUE);

VALUE
c_Cursor_get_typedef_decl_underlying_type(VALUE);

VALUE
c_Cursor_get_included_file(VALUE);

VALUE
c_Cursor_is_declaration(VALUE);

VALUE
c_Cursor_is_reference(VALUE);

VALUE
c_Cursor_is_expression(VALUE);

VALUE
c_Cursor_is_statement(VALUE);

VALUE
c_Cursor_is_attribute(VALUE);

VALUE
c_Cursor_is_invalid(VALUE);

VALUE
c_Cursor_is_translation_unit(VALUE);

VALUE
c_Cursor_is_preprocessing(VALUE);

VALUE
c_Cursor_get_enum_decl_integer_type(VALUE);

VALUE
c_Cursor_get_enum_const_decl_value(VALUE);

VALUE
c_Cursor_get_enum_const_decl_unsigned_value(VALUE);

VALUE
c_Cursor_get_field_decl_bit_width(VALUE);

VALUE
c_Cursor_get_num_arguments(VALUE);

VALUE
c_Cursor_get_argument(VALUE, VALUE);

VALUE
c_Cursor_get_num_template_arguments(VALUE);

VALUE
c_Cursor_get_decl_obj_c_type_encoding(VALUE);

VALUE
c_Cursor_get_result_type(VALUE);

VALUE
c_Cursor_get_offset_of_field(VALUE);

VALUE
c_Cursor_is_anonymous(VALUE);

VALUE
c_Cursor_is_bit_field(VALUE);

VALUE
c_Cursor_is_virtual_base(VALUE);

VALUE
c_Cursor_get_cxx_access_specifier(VALUE);

VALUE
c_Cursor_get_storage_class(VALUE);

VALUE
c_Cursor_get_num_overloaded_decls(VALUE);

VALUE
c_Cursor_get_overloaded_decl(VALUE, VALUE);

VALUE
c_Cursor_get_ib_outlet_collection_type(VALUE);

VALUE
c_Cursor_get_usr(VALUE);

VALUE
c_Cursor_get_display_name(VALUE);

VALUE
c_Cursor_get_referenced(VALUE);

VALUE
c_Cursor_get_definition(VALUE);

VALUE
c_Cursor_is_definition(VALUE);

VALUE
c_Cursor_get_canonical_cursor(VALUE);

VALUE
c_Cursor_get_obj_c_selector_index(VALUE);

VALUE
c_Cursor_is_dynamic_call(VALUE);

VALUE
c_Cursor_get_receiver_type(VALUE);

VALUE
c_Cursor_get_obj_c_decl_qualifiers(VALUE);

VALUE
c_Cursor_is_obj_c_optional(VALUE);

VALUE
c_Cursor_is_variadic(VALUE);

VALUE
c_Cursor_get_comment_range(VALUE);

VALUE
c_Cursor_get_raw_comment_text(VALUE);

VALUE
c_Cursor_get_brief_comment_text(VALUE);

VALUE
c_Cursor_get_mangling(VALUE);

VALUE
c_Cursor_cxx_method_is_pure_virtual(VALUE);

VALUE
c_Cursor_cxx_method_is_static(VALUE);

VALUE
c_Cursor_cxx_method_is_virtual(VALUE);

VALUE
c_Cursor_cxx_method_is_const(VALUE);

VALUE
c_Cursor_get_template_cursor_kind(VALUE);

VALUE
c_Cursor_get_specialized_cursor_template(VALUE);

VALUE
c_Cursor_get_completion_string(VALUE);

VALUE
c_Cursor_get_translation_unit(VALUE);
#endif //CURSOR_H
