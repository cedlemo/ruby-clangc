
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
#ifndef MODULE_H
#define MODULE_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct Module_t {
  CXModule data; 
  VALUE parent;
} Module_t;

VALUE
c_Module_struct_alloc(VALUE);

VALUE
c_Module_get_ast_file(VALUE);

VALUE
c_Module_get_parent(VALUE);

VALUE
c_Module_get_name(VALUE);

VALUE
c_Module_get_full_name(VALUE);
#endif //MODULE_H
