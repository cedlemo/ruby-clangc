
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
#include "class_SourceLocation.h"

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

/**
* call-seq:
*   Clangc::SourceRange#is_null => true or false
*
* Returns true is the underlying CXSourceRange is NULL or false otherwise
*/
VALUE
c_SourceRange_is_null(VALUE self)
{
  SourceRange_t *s;
  Data_Get_Struct(self, SourceRange_t, s);
  return NOT_0_2_RVAL(clang_Range_isNull(s->data));
}

/**
* call-seq:
*   clangc::SourceRange#is_equal
*
* Determine whether two ranges are equivalent.
* Returns true if the ranges are the same, false if they differ.
*/
VALUE
c_SourceRange_is_equal(VALUE self, VALUE other_source_range)
{
  SourceRange_t * sr1;
  SourceRange_t * sr2;
  Data_Get_Struct(self, SourceRange_t, sr1);
  Data_Get_Struct(other_source_range, SourceRange_t, sr2);
  return NOT_0_2_RVAL(clang_equalRanges(sr1->data, sr2->data));
}

/**
* call-seq:
*   Clangc::SourceRange#start => clangc::SourceLocation
*
* Retrieve a source location representing the first character within a
* source range.
*/
VALUE
c_SourceRange_get_start(VALUE self)
{
  SourceRange_t *sr;
  SourceLocation_t *sl;
  VALUE a_source_location;
  Data_Get_Struct(self, SourceRange_t, sr);

  R_GET_CLASS_DATA1("Clangc", SourceLocation, a_source_location, sl);
  sl->data = clang_getRangeStart(sr->data);
  return a_source_location;
}

/**
* call-seq:
*   Clangc::SourceRange#end => clangc::SourceLocation
*
* Retrieve a source location representing the last character within a
* source range.
*/
VALUE
c_SourceRange_get_end(VALUE self)
{
  SourceRange_t *sr;
  SourceLocation_t *sl;
  VALUE a_source_location;
  Data_Get_Struct(self, SourceRange_t, sr);

  R_GET_CLASS_DATA1("Clangc", SourceLocation, a_source_location, sl);
  sl->data = clang_getRangeEnd(sr->data);
  return a_source_location;
}
