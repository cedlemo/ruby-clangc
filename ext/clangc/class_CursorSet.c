
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
/*CursorSet ruby class*/
#include "class_CursorSet.h"
#include "macros.h"

static void
c_CursorSet_struct_free(CursorSet_t *s)
{
  if(s)
  {
    if(s->data)
      clang_disposeCXCursorSet(s->data);

    ruby_xfree(s);
  }
}  

static void
c_CursorSet_mark(void *s)
{
  if(s)
  {
    CursorSet_t *t =(CursorSet_t *)s;
    rb_gc_mark(t->parent);
  }
}
VALUE
c_CursorSet_struct_alloc( VALUE klass)
{
  
  CursorSet_t * ptr;
  ptr = (CursorSet_t *) ruby_xmalloc(sizeof(CursorSet_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_CursorSet_struct_free, (void *) ptr);
}

/**
* call-seq:
*   Clangc::CursorSet#new => Clangc::CursorSet 
*
* Creates an empty Clangc::CursorSet instance which is
* a fast container representing a set of Clangc::Cursor.
*/
VALUE
c_CursorSet_initialize(VALUE self) {
  CursorSet_t *c;
  Data_Get_Struct(self, CursorSet_t, c);
  c->data = clang_createCXCursorSet();
  return self;
}

/*VALUE
generate_CursorSet_under(VALUE module, VALUE superclass)
{
  VALUE klass = rb_define_class_under(module, "CursorSet", superclass);
  rb_define_alloc_func(klass, c_CursorSet_struct_alloc);
  return klass;
}
*/
