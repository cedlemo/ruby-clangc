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
#include "_clangc_functions.h"
#include "class_Index.h"
#include "class_TranslationUnit.h"
#include "class_Diagnostic.h"
#include "class_File.h"
#include "class_SourceRange.h"
#include "class_SourceLocation.h"
#include "class_Cursor.h"
#include "class_Type.h"
#include "class_CursorSet.h"
#include "class_CompletionString.h"
#include "class_OverriddenCursor.h"
#include "class_Module.h"

void Init_clangc(void) {
  VALUE m_Clangc = rb_define_module("Clangc");

  rb_define_module_function(m_Clangc, "version", RUBY_METHOD_FUNC(m_clangc_get_version), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_diagnostic_display_options", RUBY_METHOD_FUNC(m_clangc_get_default_diagnostic_display_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_editing_translation_unit_options", RUBY_METHOD_FUNC(m_clangc_get_default_editing_translation_unit_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_code_complete_options", RUBY_METHOD_FUNC(m_clangc_get_default_code_complete_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_source_range", RUBY_METHOD_FUNC(m_clangc_get_null_source_range), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_source_location", RUBY_METHOD_FUNC(m_clangc_get_null_source_location), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_cursor", RUBY_METHOD_FUNC(m_clangc_get_null_cursor), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "visit_children_with_proc", RUBY_METHOD_FUNC(m_clangc_visit_children_with_proc), 2);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "visit_children_with_block", RUBY_METHOD_FUNC(m_clangc_visit_children_with_block), 1);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "range", RUBY_METHOD_FUNC(m_clangc_get_range), 2);// in _clangc_functions.c

  init_clang_enums_to_constants(m_Clangc);
  init_clang_errors_enums_to_constants(m_Clangc);
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
  rb_define_method(c_Index, "create_translation_unit2", RUBY_METHOD_FUNC(c_Index_create_TU2), 1);// in class_Index.c
  rb_define_method(c_Index, "parse_translation_unit", RUBY_METHOD_FUNC(c_Index_parse_TU), 3);// in class_Index.c
  rb_define_method(c_Index, "parse_translation_unit2", RUBY_METHOD_FUNC(c_Index_parse_TU2), 3);// in class_Index.c

/*
* A single TranslationUnit which resides in an Index
*/
  VALUE c_TranslationUnit = rb_define_class_under(m_Clangc, "TranslationUnit", rb_cObject);
  rb_define_alloc_func(c_TranslationUnit, c_TranslationUnit_struct_alloc);
  rb_define_method(c_TranslationUnit, "diagnostics_num", RUBY_METHOD_FUNC(c_TranslationUnit_get_diagnostics_num), 0);// in class_TranslationUnit.c
  rb_define_method(c_TranslationUnit, "default_save_options", RUBY_METHOD_FUNC(c_TranslationUnit_get_default_save_options), 0);// in class_TranslationUnit.c
  rb_define_method(c_TranslationUnit, "spelling", RUBY_METHOD_FUNC(c_TranslationUnit_get_spelling), 0);// in class_TranslationUnit.c
  rb_define_method(c_TranslationUnit, "default_reparse_options", RUBY_METHOD_FUNC(c_TranslationUnit_get_default_reparse_options), 0);// in class_TranslationUnit.c
  rb_define_method(c_TranslationUnit, "diagnostic", RUBY_METHOD_FUNC(c_TranslationUnit_get_diagnostic), 1);// in class_TranslationUnit.c  
  rb_define_method(c_TranslationUnit, "file", RUBY_METHOD_FUNC(c_TranslationUnit_get_file), 1);// in class_TranslationUnit.c  
  rb_define_method(c_TranslationUnit, "cursor", RUBY_METHOD_FUNC(c_TranslationUnit_get_cursor), 0);// in class_TranslationUnit.c
  rb_define_method(c_TranslationUnit, "module", RUBY_METHOD_FUNC(c_TranslationUnit_get_module), 1);// in class_TranslationUnit.c

/*
* A diagnostic object, containing the diagnostic's severity,
* location, text, source ranges, and fix-it hints.
*/
  VALUE c_Diagnostic = rb_define_class_under(m_Clangc, "Diagnostic", rb_cObject);
  rb_define_alloc_func(c_Diagnostic, c_Diagnostic_struct_alloc);
  rb_define_method(c_Diagnostic, "severity", RUBY_METHOD_FUNC(c_Diagnostic_get_severity), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "spelling", RUBY_METHOD_FUNC(c_Diagnostic_get_spelling), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "category", RUBY_METHOD_FUNC(c_Diagnostic_get_category), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "category_name", RUBY_METHOD_FUNC(c_Diagnostic_get_category_name), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "category_text", RUBY_METHOD_FUNC(c_Diagnostic_get_category_text), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "num_ranges", RUBY_METHOD_FUNC(c_Diagnostic_get_num_ranges), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "num_fixits", RUBY_METHOD_FUNC(c_Diagnostic_get_num_fixits), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "format", RUBY_METHOD_FUNC(c_Diagnostic_format), 1);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "option", RUBY_METHOD_FUNC(c_Diagnostic_get_option), 0);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "source_range", RUBY_METHOD_FUNC(c_Diagnostic_get_source_range), 1);// in class_Diagnostic.c
  rb_define_method(c_Diagnostic, "source_location", RUBY_METHOD_FUNC(c_Diagnostic_get_source_location), 0);// in class_Diagnostic.c

/*
*  A particular source file that is part of a translation unit
*/

  VALUE c_File = rb_define_class_under(m_Clangc, "File", rb_cObject);
  rb_define_alloc_func(c_File, c_File_struct_alloc);
  rb_define_method(c_File, "name", RUBY_METHOD_FUNC(c_File_get_name),0);// in class_File.c
  rb_define_method(c_File, "mtime", RUBY_METHOD_FUNC(c_File_get_mtime),0);// in class_File.c
  rb_define_method(c_File, "is_multiple_include_guarded", RUBY_METHOD_FUNC(c_File_is_multiple_include_guarded),0);// in class_File.c
#if (CINDEX_VERSION_MINOR >= 29)
    rb_define_method(c_File, "is_equal", RUBY_METHOD_FUNC(c_File_is_equal), 1);// in class_File.c
#endif
/*
* Identifies a half-open character range in the source code.
*
* Use Clangc::Range#start and Clangc::Range#end to retrieve the
* starting and end locations from a source range, respectively.
*/

  VALUE c_SourceRange = rb_define_class_under(m_Clangc, "SourceRange", rb_cObject);
  rb_define_alloc_func(c_SourceRange, c_SourceRange_struct_alloc);
  rb_define_method(c_SourceRange, "is_null", RUBY_METHOD_FUNC(c_SourceRange_is_null), 0);// in class_SourceRange.c
  rb_define_method(c_SourceRange, "is_equal", RUBY_METHOD_FUNC(c_SourceRange_is_equal), 1);// in class_SourceRange.c
  rb_define_method(c_SourceRange, "start", RUBY_METHOD_FUNC(c_SourceRange_get_start), 0);// in class_SourceRange.c
  rb_define_method(c_SourceRange, "end", RUBY_METHOD_FUNC(c_SourceRange_get_end), 0);// in class_SourceRange.c

/**
* Identifies a specific source location within a translation
* unit.
*
* Use Clangc::SourceLocation#expansion or Clangc::SourceLocation#spelling
* to map a source location to a particular file, line, and column.
*/

  VALUE c_SourceLocation = rb_define_class_under(m_Clangc, "SourceLocation", rb_cObject);
  rb_define_alloc_func(c_SourceLocation, c_SourceLocation_struct_alloc);
  rb_define_method(c_SourceLocation, "is_in_system_header", RUBY_METHOD_FUNC(c_SourceLocation_is_in_system_header), 0);// in class_SourceLocation.c
  rb_define_method(c_SourceLocation, "is_from_main_file", RUBY_METHOD_FUNC(c_SourceLocation_is_from_main_file), 0);// in class_SourceLocation.c
  rb_define_method(c_SourceLocation, "is_equal", RUBY_METHOD_FUNC(c_SourceLocation_is_equal), 1);// in class_SourceLocation.c
  rb_define_method(c_SourceLocation, "spelling", RUBY_METHOD_FUNC(c_SourceLocation_get_spelling), 0);// in class_SourceLocation.c
  rb_define_method(c_SourceLocation, "file_location", RUBY_METHOD_FUNC(c_SourceLocation_get_file_location), 0);// in class_SourceLocation.c

/**
* A cursor representing some element in the abstract syntax tree for
* a translation unit.
*
* The cursor abstraction unifies the different kinds of entities in a
* program--declaration, statements, expressions, references to declarations,
* etc.--under a single "cursor" abstraction with a common set of operations.
* Common operation for a cursor include: getting the physical location in
* a source file where the cursor points, getting the name associated with a
* cursor, and retrieving cursors for any child nodes of a particular cursor.
*
* Cursors can be produced in two specific ways.
* clangc::TranslationUnit.cursor produces a cursor for a translation unit,
* from which one can use clang.visitChildren to explore the rest of the
* translation unit. clang.getCursor maps from a physical source location
* to the entity that resides at that location, allowing one to map from the
* source code into the AST.
*/

  VALUE c_Cursor = rb_define_class_under(m_Clangc, "Cursor", rb_cObject);
  rb_define_alloc_func(c_Cursor, c_Cursor_struct_alloc);
  rb_define_method(c_Cursor, "is_null", RUBY_METHOD_FUNC(c_Cursor_is_null), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_equal", RUBY_METHOD_FUNC(c_Cursor_is_equal), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "hash", RUBY_METHOD_FUNC(c_Cursor_get_hash), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "kind", RUBY_METHOD_FUNC(c_Cursor_get_kind), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "linkage", RUBY_METHOD_FUNC(c_Cursor_get_linkage), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "availability", RUBY_METHOD_FUNC(c_Cursor_get_availability), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "language", RUBY_METHOD_FUNC(c_Cursor_get_language), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "type", RUBY_METHOD_FUNC(c_Cursor_get_type), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "semantic_parent", RUBY_METHOD_FUNC(c_Cursor_get_semantic_parent), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "lexical_parent", RUBY_METHOD_FUNC(c_Cursor_get_lexical_parent), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "location", RUBY_METHOD_FUNC(c_Cursor_get_source_location), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "extent", RUBY_METHOD_FUNC(c_Cursor_get_extent), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "spelling", RUBY_METHOD_FUNC(c_Cursor_get_spelling), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "typedef_decl_underlying_type", RUBY_METHOD_FUNC(c_Cursor_get_typedef_decl_underlying_type), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "included_file", RUBY_METHOD_FUNC(c_Cursor_get_included_file), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_declaration", RUBY_METHOD_FUNC(c_Cursor_is_declaration), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_reference", RUBY_METHOD_FUNC(c_Cursor_is_reference), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_expression", RUBY_METHOD_FUNC(c_Cursor_is_expression), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_statement", RUBY_METHOD_FUNC(c_Cursor_is_statement), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_attribute", RUBY_METHOD_FUNC(c_Cursor_is_attribute), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_invalid", RUBY_METHOD_FUNC(c_Cursor_is_invalid), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_translation_unit", RUBY_METHOD_FUNC(c_Cursor_is_translation_unit), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_preprocessing", RUBY_METHOD_FUNC(c_Cursor_is_preprocessing), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "enum_decl_integer_type", RUBY_METHOD_FUNC(c_Cursor_get_enum_decl_integer_type), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "enum_const_decl_value", RUBY_METHOD_FUNC(c_Cursor_get_enum_const_decl_value), 0);// in class_Cursor.c  
  rb_define_method(c_Cursor, "enum_const_decl_unsigned_value", RUBY_METHOD_FUNC(c_Cursor_get_enum_const_decl_unsigned_value), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "field_decl_bit_width", RUBY_METHOD_FUNC(c_Cursor_get_field_decl_bit_width), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "num_arguments", RUBY_METHOD_FUNC(c_Cursor_get_num_arguments), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "argument", RUBY_METHOD_FUNC(c_Cursor_get_argument), 1);// in class_Cursor.c
#if (CINDEX_VERSION_MINOR >= 29)
  rb_define_method(c_Cursor, "num_template_arguments", RUBY_METHOD_FUNC(c_Cursor_get_num_template_arguments), 0);// in class_Cursor.c
#endif
  rb_define_method(c_Cursor, "decl_obj_c_type_encoding", RUBY_METHOD_FUNC(c_Cursor_get_decl_obj_c_type_encoding), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "result_type", RUBY_METHOD_FUNC(c_Cursor_get_result_type), 0);// in class_Cursor.c
#if (CINDEX_VERSION_MINOR >= 30)
  rb_define_method(c_Cursor, "offset_of_field", RUBY_METHOD_FUNC(c_Cursor_get_offset_of_field), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_anonymous", RUBY_METHOD_FUNC(c_Cursor_is_anonymous), 0);// in class_Cursor.c
#endif
  rb_define_method(c_Cursor, "is_bit_field", RUBY_METHOD_FUNC(c_Cursor_is_bit_field), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_virtual_base", RUBY_METHOD_FUNC(c_Cursor_is_virtual_base), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "cxx_access_specifier", RUBY_METHOD_FUNC(c_Cursor_get_cxx_access_specifier), 0);// in class_Cursor.c
#if (CINDEX_VERSION_MINOR >= 29)
  rb_define_method(c_Cursor, "storage_class", RUBY_METHOD_FUNC(c_Cursor_get_storage_class), 0);// in class_Cursor.c
#endif
  rb_define_method(c_Cursor, "num_overloaded_decls", RUBY_METHOD_FUNC(c_Cursor_get_num_overloaded_decls), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "overloaded_decl", RUBY_METHOD_FUNC(c_Cursor_get_overloaded_decl), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "ib_outlet_collection_type", RUBY_METHOD_FUNC(c_Cursor_get_ib_outlet_collection_type), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "usr", RUBY_METHOD_FUNC(c_Cursor_get_usr), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "display_name", RUBY_METHOD_FUNC(c_Cursor_get_display_name), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "referenced", RUBY_METHOD_FUNC(c_Cursor_get_referenced), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "definition", RUBY_METHOD_FUNC(c_Cursor_get_definition), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_definition", RUBY_METHOD_FUNC(c_Cursor_is_definition), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "canonical_cursor", RUBY_METHOD_FUNC(c_Cursor_get_canonical_cursor), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "obj_c_selector_index", RUBY_METHOD_FUNC(c_Cursor_get_obj_c_selector_index), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_dynamic_call", RUBY_METHOD_FUNC(c_Cursor_is_dynamic_call), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "receiver_type", RUBY_METHOD_FUNC(c_Cursor_get_receiver_type), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "obj_c_decl_qualifiers", RUBY_METHOD_FUNC(c_Cursor_get_obj_c_decl_qualifiers), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_obj_c_optional", RUBY_METHOD_FUNC(c_Cursor_is_obj_c_optional), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "is_variadic", RUBY_METHOD_FUNC(c_Cursor_is_variadic), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "comment_range", RUBY_METHOD_FUNC(c_Cursor_get_comment_range), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "raw_comment_text", RUBY_METHOD_FUNC(c_Cursor_get_raw_comment_text), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "brief_comment_text", RUBY_METHOD_FUNC(c_Cursor_get_brief_comment_text), 0);// in class_Cursor.c
#if (CINDEX_VERSION_MINOR >= 29)
  rb_define_method(c_Cursor, "mangling", RUBY_METHOD_FUNC(c_Cursor_get_mangling), 0);// in class_Cursor.c
#endif
  rb_define_method(c_Cursor, "cxx_method_is_pure_virtual", RUBY_METHOD_FUNC(c_Cursor_cxx_method_is_pure_virtual), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "cxx_method_is_static", RUBY_METHOD_FUNC(c_Cursor_cxx_method_is_static), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "cxx_method_is_virtual", RUBY_METHOD_FUNC(c_Cursor_cxx_method_is_virtual), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "cxx_method_is_const", RUBY_METHOD_FUNC(c_Cursor_cxx_method_is_const), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "template_cursor_kind", RUBY_METHOD_FUNC(c_Cursor_get_template_cursor_kind), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "specialized_cursor_template", RUBY_METHOD_FUNC(c_Cursor_get_specialized_cursor_template), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "completion_string", RUBY_METHOD_FUNC(c_Cursor_get_completion_string), 0);// in class_Cursor.c
#if (CINDEX_VERSION_MINOR >= 29)
  rb_define_method(c_Cursor, "template_argument_kind", RUBY_METHOD_FUNC(c_Cursor_get_template_argument_kind), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "template_argument_type", RUBY_METHOD_FUNC(c_Cursor_get_template_argument_type), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "template_argument_value", RUBY_METHOD_FUNC(c_Cursor_get_template_argument_value), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "template_argument_unsigned_value", RUBY_METHOD_FUNC(c_Cursor_get_template_argument_unsigned_value), 1);// in class_Cursor.c
#endif
  rb_define_method(c_Cursor, "obj_c_property_attributes", RUBY_METHOD_FUNC(c_Cursor_get_obj_c_property_attributes), 1);// in class_Cursor.c
  rb_define_method(c_Cursor, "overridden_cursors", RUBY_METHOD_FUNC(c_Cursor_get_overridden_cursors), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "module", RUBY_METHOD_FUNC(c_Cursor_get_module), 0);// in class_Cursor.c
  rb_define_method(c_Cursor, "spelling_name_range", RUBY_METHOD_FUNC(c_Cursor_get_spelling_name_range), 2);// in class_Cursor.c
  rb_define_method(c_Cursor, "reference_name_range", RUBY_METHOD_FUNC(c_Cursor_get_reference_name_range), 2);// in class_Cursor.c

/*
 * Overriden Cursor is a subclass of cursor created because they need to be freed
 * unlike the basics Clangc::Cursor
 * */

  VALUE c_OverriddenCursor = rb_define_class_under(m_Clangc, "OverriddenCursor", c_Cursor);
  rb_define_alloc_func(c_OverriddenCursor, c_OverriddenCursor_struct_alloc);

  
/*
* Type informations for Clangc::Cursor
*/

  VALUE c_Type = rb_define_class_under(m_Clangc, "Type", rb_cObject);
  rb_define_alloc_func(c_Type, c_Type_struct_alloc);
  rb_define_method(c_Type, "kind", RUBY_METHOD_FUNC(c_Type_get_kind), 0);// in class_Type.c
  rb_define_method(c_Type, "spelling", RUBY_METHOD_FUNC(c_Type_get_spelling), 0);// in class_Type.c
  rb_define_method(c_Type, "is_equal", RUBY_METHOD_FUNC(c_Type_is_equal), 1);// in class_Type.c
  rb_define_method(c_Type, "canonical_type", RUBY_METHOD_FUNC(c_Type_get_canonical_type), 0);// in class_Type.c
  rb_define_method(c_Type, "pointee_type", RUBY_METHOD_FUNC(c_Type_get_pointee_type), 0);// in class_Type.c
  rb_define_method(c_Type, "is_const_qualified", RUBY_METHOD_FUNC(c_Type_is_const_qualified), 0);// in class_Type.c
  rb_define_method(c_Type, "is_volatile_qualified", RUBY_METHOD_FUNC(c_Type_is_volatile_qualified), 0);// in class_Type.c
  rb_define_method(c_Type, "is_restrict_qualified", RUBY_METHOD_FUNC(c_Type_is_restrict_qualified), 0);// in class_Type.c
  rb_define_method(c_Type, "result_type", RUBY_METHOD_FUNC(c_Type_get_result_type), 0);// in class_Type.c
  rb_define_method(c_Type, "calling_conv", RUBY_METHOD_FUNC(c_Type_get_calling_conv), 0);// in class_Type.c
  rb_define_method(c_Type, "num_arg_types", RUBY_METHOD_FUNC(c_Type_get_num_arg_types), 0);// in class_Type.c
  rb_define_method(c_Type, "arg_type", RUBY_METHOD_FUNC(c_Type_get_arg_type), 1);// in class_Type.c
  rb_define_method(c_Type, "element_type", RUBY_METHOD_FUNC(c_Type_get_element_type), 0);// in class_Type.c
  rb_define_method(c_Type, "num_elements", RUBY_METHOD_FUNC(c_Type_get_num_elements), 0);// in class_Type.c
  rb_define_method(c_Type, "array_element_type", RUBY_METHOD_FUNC(c_Type_get_array_element_type), 0);// in class_Type.c
  rb_define_method(c_Type, "array_size", RUBY_METHOD_FUNC(c_Type_get_array_size), 0);// in class_Type.c
  rb_define_method(c_Type, "is_pod", RUBY_METHOD_FUNC(c_Type_is_pod), 0);// in class_Type.c
  rb_define_method(c_Type, "type_declaration", RUBY_METHOD_FUNC(c_Type_get_type_declaration), 0);// in class_Type.c
  rb_define_method(c_Type, "is_function_type_variadic", RUBY_METHOD_FUNC(c_Type_is_function_type_variadic), 0);// in class_Type.c
  rb_define_method(c_Type, "align_of", RUBY_METHOD_FUNC(c_Type_get_align_of), 0);// in class_Type.c
  rb_define_method(c_Type, "size_of", RUBY_METHOD_FUNC(c_Type_get_size_of), 0);// in class_Type.c
  rb_define_method(c_Type, "class_type", RUBY_METHOD_FUNC(c_Type_get_class_type), 0);// in class_Type.c
  rb_define_method(c_Type, "offset_of", RUBY_METHOD_FUNC(c_Type_get_offset_of), 1);// in class_Type.c
  rb_define_method(c_Type, "num_template_arguments", RUBY_METHOD_FUNC(c_Type_get_num_template_arguments), 0);// in class_Type.c
  rb_define_method(c_Type, "template_argument_as_type", RUBY_METHOD_FUNC(c_Type_get_template_argument_as_type), 1);// in class_Type.c
  rb_define_method(c_Type, "cxx_ref_qualifier", RUBY_METHOD_FUNC(c_Type_get_cxx_ref_qualifier), 0);// in class_Type.c
 
/**
* \brief A fast container representing a set of CXCursors.
*/
  VALUE c_CursorSet = rb_define_class_under(m_Clangc, "CursorSet", rb_cObject);
  rb_define_alloc_func(c_CursorSet, c_CursorSet_struct_alloc);
  rb_define_private_method(c_CursorSet, "initialize", RUBY_METHOD_FUNC(c_CursorSet_initialize), 0);// in class_CursorSet.c
  rb_define_method(c_CursorSet, "contains", RUBY_METHOD_FUNC(c_CursorSet_contains), 1);// in class_CursorSet.c
  rb_define_method(c_CursorSet, "insert", RUBY_METHOD_FUNC(c_CursorSet_insert), 1);// in class_CursorSet.c

/**
* \brief A semantic string that describes a code-completion result.
*
* A semantic string that describes the formatting of a code-completion
* result as a single "template" of text that should be inserted into the
* source buffer when a particular code-completion result is selected.
* Each semantic string is made up of some number of "chunks", each of which
* contains some text along with a description of what that text means, e.g.,
* the name of the entity being referenced, whether the text chunk is part of
* the template, or whether it is a "placeholder" that the user should replace
* with actual code,of a specific kind. See \c CXCompletionChunkKind for a
* description of the different kinds of chunks.
*/
  VALUE c_CompletionString = rb_define_class_under(m_Clangc, "CompletionString", rb_cObject);
  rb_define_alloc_func(c_CompletionString, c_CompletionString_struct_alloc);
  rb_define_method(c_CompletionString, "availability", RUBY_METHOD_FUNC(c_CompletionString_get_availability), 0);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "priority", RUBY_METHOD_FUNC(c_CompletionString_get_priority), 0);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "num_completion_chunks", RUBY_METHOD_FUNC(c_CompletionString_get_num_completion_chunks), 0);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "completion_chunk_kind", RUBY_METHOD_FUNC(c_CompletionString_get_completion_chunk_kind), 1);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "completion_chunk_text", RUBY_METHOD_FUNC(c_CompletionString_get_completion_chunk_text), 1);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "completion_num_annotations", RUBY_METHOD_FUNC(c_CompletionString_get_completion_num_annotations), 0);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "completion_annotation", RUBY_METHOD_FUNC(c_CompletionString_get_completion_annotation), 1);// in class_CompletionString.c
  rb_define_method(c_CompletionString, "brief_comment", RUBY_METHOD_FUNC(c_CompletionString_get_brief_comment), 0);// in class_CompletionString.c

/**
* CXModule class and method
*/
  VALUE c_Module = rb_define_class_under(m_Clangc, "Module", rb_cObject);
  rb_define_alloc_func(c_Module, c_Module_struct_alloc);
  rb_define_method(c_Module, "ast_file", RUBY_METHOD_FUNC(c_Module_get_ast_file), 0);// in class_Module.c
  rb_define_method(c_Module, "parent", RUBY_METHOD_FUNC(c_Module_get_parent), 0);// in class_Module.c
  rb_define_method(c_Module, "name", RUBY_METHOD_FUNC(c_Module_get_name), 0);// in class_Module.c
  rb_define_method(c_Module, "full_name", RUBY_METHOD_FUNC(c_Module_get_full_name), 0);// in class_Module.c
  rb_define_method(c_Module, "is_system", RUBY_METHOD_FUNC(c_Module_is_system), 0);// in class_Module.c
  rb_define_method(c_Module, "num_top_level_headers", RUBY_METHOD_FUNC(c_Module_get_num_top_level_headers), 1);// in class_Module.c
  rb_define_method(c_Module, "top_level_header", RUBY_METHOD_FUNC(c_Module_get_top_level_header), 2);// in class_Module.c
}
