/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016  cedlemo <cedlemo@gmx.com>
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
#ifndef TRANSLATIONUNIT_H
#define TRANSLATIONUNIT_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct TranslationUnit_t {
  CXTranslationUnit data; 
  VALUE parent;
} TranslationUnit_t;

VALUE
c_TranslationUnit_struct_alloc(VALUE);

VALUE
c_TranslationUnit_get_diagnostics_num(VALUE);

VALUE
c_TranslationUnit_get_default_save_options(VALUE);

VALUE
c_TranslationUnit_get_spelling(VALUE);

VALUE
c_TranslationUnit_get_default_reparse_options(VALUE);

VALUE
c_TranslationUnit_get_diagnostic(VALUE, VALUE);

VALUE
c_TranslationUnit_get_file(VALUE, VALUE);

VALUE
c_TranslationUnit_get_cursor(VALUE);

VALUE
c_TranslationUnit_get_module(VALUE, VALUE);
#endif //TRANSLATIONUNIT_H
