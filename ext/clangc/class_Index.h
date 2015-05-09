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
#ifndef INDEX_H
#define INDEX_H
#include <ruby/ruby.h>
#include "class_TranslationUnit.h"
typedef struct Index_t {
    CXIndex data; 
} Index_t;

VALUE
c_Index_struct_alloc(VALUE);

VALUE
c_Index_initialize(VALUE, VALUE, VALUE);

VALUE
c_Index_set_global_options(VALUE, VALUE);

VALUE
c_Index_get_global_options(VALUE);

VALUE
c_Index_create_TU_from_source_file(VALUE, VALUE, VALUE);

VALUE
c_Index_create_TU(VALUE, VALUE);

VALUE
c_Index_parse_TU(VALUE, VALUE, VALUE, VALUE);
#endif //INDEX_H
