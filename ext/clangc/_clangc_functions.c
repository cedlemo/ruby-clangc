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

#include "_clangc_functions.h"
#include "clang-c/Index.h"
#include "class_SourceRange.h"
#include "class_SourceLocation.h"

/**
* call-seq:
*   Clangc.version => String
*
* Return a version string, suitable for showing to a user, but not
* intended to be parsed (the format is not guaranteed to be stable).
*/

VALUE
m_clangc_get_version(VALUE self)
{
  CXString str = clang_getClangVersion();
  VALUE version = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return version;
}

/**
* call-seq:
*   Clangc.default_diagnostic_display_options => Fixnum
* 
* Retrieve the set of display options most similar to the
* default behavior of the clang compiler.
*
* A set of display options suitable for use with Clangc::Diagnostic#format
*/

VALUE
m_clangc_get_default_diagnostic_display_options(VALUE self)
{
  return CUINT_2_NUM(clang_defaultDiagnosticDisplayOptions());
}

/**
* call-seq:
*   Clangc.default_editing_translation_unit_options => Fixnum  
*
* Returns the set of flags that is suitable for parsing a translation
* unit that is being edited. (Clangc::TranslationUnit_Flags constants)
*
* The set of flags returned provide options for Clangc::Index#parseTranslationUnit
* to indicate that the translation unit is likely to be reparsed many times,
* either explicitly (via Clangc::TranslationUnit#reparse) or implicitly
* (e.g., by code completion ( Clangc::TranslationUnit#CodeCompleteAt). The returned flag
* set contains an unspecified set of optimizations (e.g., the precompiled 
* preamble) geared toward improving the performance of these routines. The
* set of optimizations enabled may change from one version to the next.
*/

VALUE
m_clangc_get_default_editing_translation_unit_options(VALUE self)
{
  return CUINT_2_NUM(clang_defaultEditingTranslationUnitOptions());
}

/**
* call-seq:
*   Clangc.default_code_complete_options => Fixnum
*
* Returns a default set of code-completion options that can be
* passed to Clangc::TranslationUnit#codeCompleteAt. 
* This set is be bitwise-OR'd constants of the Clangc::CodeComplete_Flags
*/

VALUE
m_clangc_get_default_code_complete_options(VALUE self)
{
  return CUINT_2_NUM(clang_defaultCodeCompleteOptions());
}

/**
* call-seq:
*   Clangc.null_source_range
*
* Retrieve a NULL (invalid) source range
*/

VALUE
m_clangc_get_null_source_range(VALUE self)
{
  SourceRange_t *s;
  VALUE a_source_range;
  R_GET_CLASS_DATA("Clangc", "SourceRange", a_source_range, SourceRange_t, s);
  s->data = clang_getNullRange();
  return a_source_range;
}

/**
* call-seq:
*   Clangc.null_source_location
*
* Retrieve a NULL (invalid) source location.
*/

VALUE
m_clangc_get_null_source_location(VALUE self)
{
  SourceLocation_t *s;
  VALUE a_source_location;
  R_GET_CLASS_DATA("Clangc", "SourceLocation", a_source_location, SourceLocation_t, s);
  s->data = clang_getNullLocation();
  return a_source_location;
}
