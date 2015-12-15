
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
#ifndef CURSORSET_H
#define CURSORSET_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct CursorSet_t {
  CXCursorSet data; 
  VALUE parent;
} CursorSet_t;

VALUE
c_CursorSet_struct_alloc(VALUE);

VALUE
c_CursorSet_initialize(VALUE);

VALUE
c_CursorSet_contains(VALUE, VALUE);
#endif //CURSORSET_H
