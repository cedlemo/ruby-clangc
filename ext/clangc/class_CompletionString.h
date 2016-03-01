
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
#ifndef COMPLETIONSTRING_H
#define COMPLETIONSTRING_H
#include <ruby/ruby.h>
#include "clang-c/Index.h"
typedef struct CompletionString_t {
  CXCompletionString data; 
  VALUE parent;
} CompletionString_t;

VALUE
c_CompletionString_struct_alloc(VALUE);

VALUE
c_CompletionString_get_availability(VALUE);

VALUE
c_CompletionString_get_priority(VALUE);

VALUE
c_CompletionString_get_num_chunks(VALUE);

VALUE
c_CompletionString_get_chunk_kind(VALUE, VALUE);

VALUE
c_CompletionString_get_chunk_text(VALUE, VALUE);

VALUE
c_CompletionString_get_num_annotations(VALUE);

VALUE
c_CompletionString_get_annotation(VALUE, VALUE);

VALUE
c_CompletionString_get_brief_comment(VALUE);

VALUE
c_CompletionString_get_chunk_completion_string(VALUE, VALUE);
#endif //COMPLETIONSTRING_H
