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
#ifndef _CLANGC_FUNCTIONS_H
#define _CLANGC_FUNCTIONS_H
#include <ruby/ruby.h>
#include "macros.h"
VALUE
m_clangc_get_version(VALUE);

VALUE
m_clangc_get_default_diagnostic_display_options(VALUE);

VALUE
m_clangc_get_default_editing_translation_unit_options(VALUE);

VALUE
m_clangc_get_default_code_complete_options(VALUE);

VALUE
m_clangc_get_null_source_range(VALUE);

VALUE
m_clangc_get_null_source_location(VALUE);

VALUE
m_clangc_get_null_cursor(VALUE);

VALUE
m_clangc_visit_children_with_proc(VALUE, VALUE, VALUE);

VALUE
m_clangc_visit_children_with_block(VALUE, VALUE);

VALUE
m_clangc_get_range(VALUE, VALUE, VALUE);
#endif
