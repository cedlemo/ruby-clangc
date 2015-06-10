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

/**
* call-seq:
*   Clangc::version => String
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
*   Clangc::default_diagnostic_display_options => Fixnum
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

