
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
/*Diagnostic ruby class*/
#include "clang-c/Index.h"
#include "class_Diagnostic.h"
#include "macros.h"

static void
c_Diagnostic_struct_free(Diagnostic_t *s)
{
  if(s)
  {
    
  if(s->data)
    clang_disposeDiagnostic(s->data); 

    ruby_xfree(s);
  }
}  
static void
c_Diagnostic_mark(void *s)
{
  if(s)
  {
    Diagnostic_t *d = (Diagnostic_t *)s;
    rb_gc_mark(d->parent);
  }
}
VALUE
c_Diagnostic_struct_alloc( VALUE klass)
{
  
    Diagnostic_t * ptr;
    ptr = (Diagnostic_t *) ruby_xmalloc(sizeof(Diagnostic_t)); 
    ptr->data = NULL;
    ptr->parent = Qnil;
  return Data_Wrap_Struct(klass, c_Diagnostic_mark, c_Diagnostic_struct_free, (void *) ptr);
}

