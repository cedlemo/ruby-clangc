/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2016  Cédric Le Moigne <cedlemo@gmx.com>
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
#ifndef CODECOMPLETERESULTS_H
#define CODECOMPLETERESULTS_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct CodeCompleteResults_t {
  CXCodeCompleteResults *data; 
  VALUE parent;
} CodeCompleteResults_t;

VALUE
c_CodeCompleteResults_struct_alloc(VALUE);

VALUE
c_CodeCompleteResults_get_num_results(VALUE);

VALUE
c_CodeCompleteResults_get_result(VALUE, VALUE);

VALUE
c_CodeCompleteResults_get_container_usr(VALUE);

VALUE
c_CodeCompleteResults_get_num_diagnostics(VALUE);

VALUE
c_CodeCompleteResults_get_diagnostic(VALUE, VALUE);

VALUE
c_CodeCompleteResults_sort_results(VALUE);
#endif //CODECOMPLETERESULTS_H
