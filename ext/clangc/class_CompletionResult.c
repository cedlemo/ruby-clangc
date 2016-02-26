
/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2016 Cédric Le Moigne <cedlemo@gmx.com>
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
/*CompletionResult ruby class*/
#include "class_CompletionResult.h"
#include "macros.h"

static void
c_CompletionResult_struct_free(CompletionResult_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_CompletionResult_mark(void *s)
{
  if(s)
  {
    CompletionResult_t *t =(CompletionResult_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_CompletionResult_struct_alloc( VALUE klass)
{
  
  CompletionResult_t * ptr;
  ptr = (CompletionResult_t *) ruby_xmalloc(sizeof(CompletionResult_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_CompletionResult_struct_free, (void *) ptr);
}
