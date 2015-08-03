
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
/*Type ruby class*/
#include "class_Type.h"
#include "macros.h"

static void
c_Type_struct_free(Type_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_Type_mark(void *s)
{
  if(s)
  {
    Type_t *t =(Type_t *)s;
    rb_gc_mark(t->parent);
  }
}
VALUE
c_Type_struct_alloc( VALUE klass)
{
  
  return Data_Wrap_Struct(klass, NULL, c_Type_struct_free, ruby_xmalloc(sizeof(Type_t)));
}
/**
* call-seq:
*   Clangc::Type#kind => Fixnum
*
* Get the kind of type. The returned value is a postive integer contained in
* Clangc::TypeKind.constants
*/
VALUE
c_Type_get_kind(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  return CUINT_2_NUM(t->data.kind);
}

/**
* call-seq:
*   Clangc::Type#spelling => String
*
* Pretty-print the underlying type using the rules of the
* language of the translation unit from which it came.
*
* If the type is invalid, an empty string is returned.
*/
VALUE
c_Type_get_spelling(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  CXString str = clang_getTypeSpelling(t->data);
  VALUE spelling = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return spelling;
}