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
#ifndef TYPE_H
#define TYPE_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct Type_t {
  CXType data; 
  VALUE parent;
} Type_t;

VALUE
c_Type_struct_alloc(VALUE);

VALUE
c_Type_get_kind(VALUE);

VALUE
c_Type_get_spelling(VALUE);

VALUE
c_Type_is_equal(VALUE, VALUE);

VALUE
c_Type_get_canonical_type(VALUE);

VALUE
c_Type_get_pointee_type(VALUE);

VALUE
c_Type_is_const_qualified(VALUE);

VALUE
c_Type_is_volatile_qualified(VALUE);

VALUE
c_Type_is_restrict_qualified(VALUE);

VALUE
c_Type_get_result_type(VALUE);

VALUE
c_Type_get_calling_conv(VALUE);

VALUE
c_Type_get_num_arg_types(VALUE);
#endif //TYPE_H
