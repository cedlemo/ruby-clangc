
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
/*Module ruby class*/
#include "class_Module.h"
#include "class_File.h"
#include "macros.h"

static void
c_Module_struct_free(Module_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_Module_mark(void *s)
{
  if(s)
  {
    Module_t *t =(Module_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_Module_struct_alloc(VALUE klass)
{
  
  Module_t * ptr;
  ptr = (Module_t *) ruby_xmalloc(sizeof(Module_t)); 
  ptr->data = NULL;
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_Module_struct_free, (void *) ptr);
}

/**
 * call-seq:
 *  Clangc::Module#ast_file => CLangc::File or nil
 *
 * Get the module file where the provided module object came from.
 */
VALUE
c_Module_get_ast_file(VALUE self)
{
  Module_t *m;
  Data_Get_Struct(self, Module_t, m);
  VALUE ast_file;
  File_t *f;
  R_GET_CLASS_DATA("Clangc", "File", ast_file, File_t, f);
  f->data = clang_Module_getASTFile(m->data);
  if (f->data)
    return ast_file;
  else
    return Qnil;
}

/**
 * call-seq:
 *  Clangc::Module#parent => Clangc::Module
 *
 * the parent of a sub-module or NULL if the given module is top-level,
 * e.g. for 'std.vector' it will return the 'std' module.
 */
VALUE
c_Module_get_parent(VALUE self)
{
  Module_t *m;
  Data_Get_Struct(self, Module_t, m);
  VALUE parent;
  Module_t *p;
  R_GET_CLASS_DATA("Clangc", "Module", parent, Module_t, p);
  p->data = clang_Module_getParent(m->data);
  return parent;
}
