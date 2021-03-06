/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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
#ifndef SOURCELOCATION_H
#define SOURCELOCATION_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct SourceLocation_t
{
    CXSourceLocation data;
    VALUE parent;
} SourceLocation_t;

VALUE
c_SourceLocation_struct_alloc(VALUE);

VALUE
c_SourceLocation_is_in_system_header(VALUE);

VALUE
c_SourceLocation_is_from_main_file(VALUE);

VALUE
c_SourceLocation_is_equal(VALUE, VALUE);

VALUE
c_SourceLocation_get_spelling(VALUE);

VALUE
c_SourceLocation_get_file_location(VALUE);
#endif // SOURCELOCATION_H
