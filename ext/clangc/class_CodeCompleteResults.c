
/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2016  cedlemo <cedlemo@gmx.com>
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
/*CodeCompleteResults ruby class*/
#include "class_CodeCompleteResults.h"
#include "macros.h"

static void
c_CodeCompleteResults_struct_free(CodeCompleteResults_t *s)
{
  if(s)
  {
    if(s->data)
      clang_disposeCodeCompleteResults(s->data);
    ruby_xfree(s);
  }
}  

static void
c_CodeCompleteResults_mark(void *s)
{
  if(s)
  {
    CodeCompleteResults_t *t =(CodeCompleteResults_t *)s;
    rb_gc_mark(t->parent);
  }
}

  VALUE
c_CodeCompleteResults_struct_alloc( VALUE klass)
{
  
  CodeCompleteResults_t * ptr;
  ptr = (CodeCompleteResults_t *) ruby_xmalloc(sizeof(CodeCompleteResults_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_CodeCompleteResults_struct_free, (void *) ptr);
}
