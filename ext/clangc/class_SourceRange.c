
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
/*SourceRange ruby class*/
#include "class_SourceRange.h"
#include "macros.h"

static void
c_SourceRange_struct_free(SourceRange_t *s)
{
  if(s)
  {
    ruby_xfree(s);
  }
}  

static void
c_SourceRange_mark(void *s)
{
  if(s)
  {
    SourceRange_t *t =(SourceRange_t *)s;
    rb_gc_mark(t->parent);
  }
}
VALUE
c_SourceRange_struct_alloc( VALUE klass)
{
  
  SourceRange_t * ptr;
  ptr = (SourceRange_t *) ruby_xmalloc(sizeof(SourceRange_t)); 
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_SourceRange_struct_free, (void *) ptr);
}
