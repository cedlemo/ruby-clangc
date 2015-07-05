
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
/*SourceLocation ruby class*/
#include "class_SourceLocation.h"
#include "macros.h"

static void
c_SourceLocation_struct_free(SourceLocation_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_SourceLocation_mark(void *s)
{
  if(s)
  {
    SourceLocation_t *t =(SourceLocation_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_SourceLocation_struct_alloc( VALUE klass)
{
  
  return Data_Wrap_Struct(klass, NULL, c_SourceLocation_struct_free, ruby_xmalloc(sizeof(SourceLocation_t)));
}

/**
* call-seq:
*   Clangc::SourceLocation#is_in_system_header => true
*
* Returns true if the given source location is in a system header.
*/
VALUE
c_SourceLocation_is_in_system_header(VALUE self)
{
  SourceLocation_t *s;
  Data_Get_Struct(self, SourceLocation_t, s);
  if(clang_Location_isInSystemHeader(s->data) > 0)
    return Qtrue;
  else
    return Qfalse;
}
