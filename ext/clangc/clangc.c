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

void Init_clangc(void) {
  VALUE m_Clangc = rb_define_module("Clangc");

  rb_define_module_function(m_Clangc, "version", RUBY_METHOD_FUNC(m_clangc_get_version), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_diagnostic_display_options", RUBY_METHOD_FUNC(m_clangc_get_default_diagnostic_display_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_editing_translation_unit_options", RUBY_METHOD_FUNC(m_clangc_get_default_editing_translation_unit_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "default_code_complete_options", RUBY_METHOD_FUNC(m_clangc_get_default_code_complete_options), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_source_range", RUBY_METHOD_FUNC(m_clangc_get_null_source_range), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_source_location", RUBY_METHOD_FUNC(m_clangc_get_null_source_location), 0);// in _clangc_functions.c
  rb_define_module_function(m_Clangc, "null_cursor", RUBY_METHOD_FUNC(m_clangc_get_null_cursor), 0);// in _clangc_functions.c

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
  rb_define_method(c_File, "is_equal", RUBY_METHOD_FUNC(c_File_is_equal), 1);// in class_File.c

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
}
