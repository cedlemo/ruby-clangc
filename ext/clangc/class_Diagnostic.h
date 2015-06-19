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
#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct Diagnostic_t {
  CXDiagnostic data; 
  VALUE parent;
} Diagnostic_t;

VALUE
c_Diagnostic_struct_alloc(VALUE);

VALUE
c_Diagnostic_get_severity(VALUE);

VALUE
c_Diagnostic_get_spelling(VALUE);

VALUE
c_Diagnostic_get_category(VALUE);

VALUE
c_Diagnostic_get_category_name(VALUE);

VALUE
c_Diagnostic_get_category_text(VALUE);

VALUE
c_Diagnostic_get_num_ranges(VALUE);

VALUE
c_Diagnostic_get_num_fixits(VALUE);

VALUE
c_Diagnostic_format(VALUE, VALUE);

VALUE
c_Diagnostic_get_option(VALUE);
#endif //DIAGNOSTIC_H
