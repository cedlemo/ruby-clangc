
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
/*File ruby class*/
#include "class_File.h"
#include "macros.h"

static void
c_File_struct_free(File_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_File_mark(void *s)
{
  if(s)
  {
    File_t *t =(File_t *)s;
    rb_gc_mark(t->parent);
  }
}
VALUE
c_File_struct_alloc( VALUE klass)
{
  
  File_t * ptr;
  ptr = (File_t *) ruby_xmalloc(sizeof(File_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;
  return Data_Wrap_Struct(klass, c_File_mark, c_File_struct_free, (void *) ptr);
}


VALUE
generate_File_under(VALUE module, VALUE superclass)
{
  VALUE klass = rb_define_class_under(module, "File", superclass);
  rb_define_alloc_func(klass, c_File_struct_alloc);
  return klass;
}

