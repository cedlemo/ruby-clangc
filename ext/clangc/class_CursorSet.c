
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
#include "class_Cursor.h"

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


/**
* call-seq:
*   Clangc::CursorSet#contains(Clangc::Cursor) => true / false
*
* Queries a CXCursorSet to see if it contains a specific CXCursor.
*
* Returns true if the set contains the specified cursor.
*/
VALUE
c_CursorSet_contains(VALUE self, VALUE cursor)
{
  CursorSet_t *cs;
  Cursor_t *c;

  Data_Get_Struct(self, CursorSet_t, cs);
  Data_Get_Struct(cursor, Cursor_t, c);

  return NOT_0_2_RVAL(clang_CXCursorSet_contains(cs->data, c->data));
}

/**
* call-seq:
*   Clangc::CursorSet(Clangc::Cursor) => true/false
*
* Inserts a CXCursor into a CXCursorSet.
*
* Returns false if the CXCursor was already in the set, and true otherwise.
*/
VALUE
c_CursorSet_insert(VALUE self, VALUE cursor)
{
  CursorSet_t *cs;
  Cursor_t *c;

  Data_Get_Struct(self, CursorSet_t, cs);
  Data_Get_Struct(cursor, Cursor_t, c);

  return NOT_0_2_RVAL(clang_CXCursorSet_insert(cs->data, c->data));
}
