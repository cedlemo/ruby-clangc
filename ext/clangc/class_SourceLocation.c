
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
#include "class_File.h"
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

/**
* call-seq:
*   Clangc::SourceLocation#is_from_main_file => true
*
* Returns true if the given source location is in the main file of
* the corresponding translation unit.
*/
VALUE
c_SourceLocation_is_from_main_file(VALUE self)
{
  SourceLocation_t *s;
  Data_Get_Struct(self, SourceLocation_t, s);
  if(clang_Location_isFromMainFile(s->data) > 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::SourceLocation#equal(Clangc::SourceLocation) => true /false
*
* Determine whether two source locations, which must refer into
* the same translation unit, refer to exactly the same point in the source
* code.
*
* true if the source locations refer to the same location, false
* if they refer to different locations.
*/
VALUE
c_SourceLocation_is_equal(VALUE self, VALUE source_location)
{
  SourceLocation_t *s;
  SourceLocation_t *sl;
  Data_Get_Struct(self, SourceLocation_t, s);
  Data_Get_Struct(source_location, SourceLocation_t, sl);
  
  if(clang_equalLocations(s->data, sl->data) > 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::SourceLocation#spelling => Array
*
* Retrieve the file, line, column, and offset represented by
* the given source location.
*
* If the location refers into a macro instantiation, return where the
* location was originally spelled in the source file.
*
* The returned array contains four values:
*
* array[0] file if not nil, will be set to the file to which the given
* source location points.(Clangc::File)
*
* array[1] if not nil, will be set to the line to which the given
* source location points.
*
* array[2] if not nil, will be set to the column to which the given
* source location points.
*
* array[3] if not nil, will be set to the offset into the
* buffer to which the given source location points. (the position
* in the file)
*/
VALUE c_SourceLocation_get_spelling(VALUE self)
{
  VALUE ret = rb_ary_new();
  SourceLocation_t *s;
  Data_Get_Struct(self, SourceLocation_t, s);
  CXFile cxf; //= malloc(sizeof(CXFile));
  unsigned int line;
  unsigned int column;
  unsigned int offset;
  clang_getSpellingLocation(s->data, &cxf, &line, &column, &offset);
  if(&cxf)
  {
    VALUE file;
    File_t *f;
    R_GET_CLASS_DATA("Clangc", "File", file, File_t, f);
    f->data = cxf;
    f->parent = self;
    rb_ary_push(ret, file);
  }
  else
    rb_ary_push(ret, Qnil);

  if(&line)
    rb_ary_push(ret, CUINT_2_NUM(line));
  else
    rb_ary_push(ret, Qnil);

  if(&column)
    rb_ary_push(ret, CUINT_2_NUM(column));
  else
    rb_ary_push(ret, Qnil);

  if(&offset)
    rb_ary_push(ret, CUINT_2_NUM(offset));
  else
    rb_ary_push(ret, Qnil);
    
  return ret;
}
/**
* call-seq:
*   Clangc::SourceLocation#file_location => Array
*
* Retrieve the file, line, column, and offset represented by
* the given source location.
*
* If the location refers into a macro instantiation, return where the
* location was originally spelled in the source file.
*
* The returned array contains four values:
*
* array[0] file if not nil, will be set to the file to which the given
* source location points.(Clangc::File)
*
* array[1] if not nil, will be set to the line to which the given
* source location points.
*
* array[2] if not nil, will be set to the column to which the given
* source location points.
*
* array[3] if not nil, will be set to the offset into the
* buffer to which the given source location points. (the position
* in the file)
*/
VALUE c_SourceLocation_get_file_location(VALUE self)
{
  VALUE ret = rb_ary_new();
  SourceLocation_t *s;
  Data_Get_Struct(self, SourceLocation_t, s);
  CXFile cxf; //= malloc(sizeof(CXFile));
  unsigned int line;
  unsigned int column;
  unsigned int offset;
  clang_getFileLocation(s->data, &cxf, &line, &column, &offset);
  if(&cxf)
  {
    VALUE file;
    File_t *f;
    R_GET_CLASS_DATA("Clangc", "File", file, File_t, f);
    f->data = cxf;
    f->parent = self;
    rb_ary_push(ret, file);
  }
  else
    rb_ary_push(ret, Qnil);

  if(&line)
    rb_ary_push(ret, CUINT_2_NUM(line));
  else
    rb_ary_push(ret, Qnil);

  if(&column)
    rb_ary_push(ret, CUINT_2_NUM(column));
  else
    rb_ary_push(ret, Qnil);

  if(&offset)
    rb_ary_push(ret, CUINT_2_NUM(offset));
  else
    rb_ary_push(ret, Qnil);
    
  return ret;
}