/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016  cedlemo <cedlemo@gmx.com>
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
/*TranslationUnit ruby class*/
#include "class_TranslationUnit.h"
#include "class_Diagnostic.h"
#include "class_File.h"
#include "class_Cursor.h"
#include "class_Module.h"
#include "macros.h"

static void
c_TranslationUnit_struct_free(TranslationUnit_t *s)
{
  if(s)
  {
    
  if(s->data)
    clang_disposeTranslationUnit(s->data); 

    ruby_xfree(s);
  }
}  
static void
c_TranslationUnit_mark(void *s)
{
  if(s)
  {
    TranslationUnit_t *t =(TranslationUnit_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_TranslationUnit_struct_alloc( VALUE klass)
{
  
    TranslationUnit_t * ptr;
    ptr = (TranslationUnit_t *) ruby_xmalloc(sizeof(TranslationUnit_t)); 
    ptr->data = NULL;
    ptr->parent = Qnil;
  return Data_Wrap_Struct(klass, c_TranslationUnit_mark, c_TranslationUnit_struct_free, (void *) ptr);
}

/*
* call-seq:
*   Clangc::TranslationUnit#diagnostics_num => num
*
* Determine the number of diagnostics produced for the given
* translation unit.
*/
VALUE
c_TranslationUnit_get_diagnostics_num(VALUE self)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  unsigned int num = clang_getNumDiagnostics(t->data);
  return CUINT_2_NUM(num); 
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#default_save_options => num
 *
 * Returns the set of flags that is suitable for saving a translation
 * unit. Those flags should be Clangc::SaveTranslationUnit_Flags constant
 *
 * The set of flags returned provide options for Clangc::TranslationUnit#save by default.
 * The returned flags set contains an unspecified set of options that save translation units with
 * the most commonly-requested data.
 */
VALUE
c_TranslationUnit_get_default_save_options(VALUE self)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  unsigned int num = clang_defaultSaveOptions(t->data);
  return CUINT_2_NUM(num);
}

/**
* call-seq:
*   Clangc::TranslationUnit#spelling => string
*
* Get the original translation unit source file name. 
*/
VALUE
c_TranslationUnit_get_spelling(VALUE self)
{
  VALUE spelling = Qnil;
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  return CXSTR_2_RVAL(clang_getTranslationUnitSpelling(t->data));
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#default_reparse_options => num
 *
 * Returns the set of flags that is suitable for reparsing a translation
 * unit.
 *
 * The set of flags returned provide options for clang_reparseTranslationUnit()
 * by default. The returned flag set contains an unspecified set of optimizations 
 * geared toward common uses of reparsing. The set of optimizations enabled may 
 * change from one version  to the next. Clangc::ReparseF_Flags constants.
 */

VALUE
c_TranslationUnit_get_default_reparse_options(VALUE self)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  unsigned int num = clang_defaultReparseOptions(t->data);
  return CUINT_2_NUM(num);
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#diagnostic(index) => Clangc::Diagnostic
 *
 * Retrieve a diagnostic associated with the given translation unit.
 *
 * index the zero-based diagnostic number to retrieve.
 *
 * returns the requested diagnostic.
 */
VALUE
c_TranslationUnit_get_diagnostic(VALUE self, VALUE num)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  unsigned int max = clang_getNumDiagnostics(t->data);
  unsigned int c_num = NUM2UINT(num);
  CHECK_IN_RANGE(c_num, 0, max);
  VALUE diagnostic;
  Diagnostic_t *d;
  R_GET_CLASS_DATA("Clangc", "Diagnostic", diagnostic, Diagnostic_t, d); 
  d->data = clang_getDiagnostic(t->data, c_num);
  d->parent = self;
  return diagnostic;
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#file(file_name) => clangc::File or nil
 *
 * Retrieve a file handle within the given translation unit.
 * file_name a String for the name of the file.
 *
 * it returns the file handle for the named file in the translation unit ,
 * a nil if the file was not a part of this translation unit.
 */
VALUE
c_TranslationUnit_get_file(VALUE self, VALUE file_name)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  CXFile cxfile;
  char * c_file_name;
  RSTRING_2_CHAR(file_name, c_file_name);
  cxfile = clang_getFile(t->data, c_file_name); 
 
  if(cxfile)
  {  
    VALUE file;
    File_t *f;
    R_GET_CLASS_DATA("Clangc", "File", file, File_t, f);
    f->data = cxfile;
    f->parent = self;
    return file;
  }
  else
    return Qnil;
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#cursor => Clangc::Cursor
 *
 * Retrieve the cursor that represents the given translation unit.
 *
 * The translation unit cursor can be used to start traversing the
 * various declarations within the given translation unit.
 */
VALUE
c_TranslationUnit_get_cursor(VALUE self)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  Cursor_t *c;
  VALUE a_cursor;
  R_GET_CLASS_DATA("Clangc", "Cursor", a_cursor, Cursor_t, c);
  c->data = clang_getTranslationUnitCursor(t->data);
  c->parent = self;
  return a_cursor;
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#module(Clangc::File) => Clangc::Module
 *
 * Given a Clangc::File header file, return the module that contains it, if one
 * exists.
 */
VALUE
c_TranslationUnit_get_module(VALUE self, VALUE file)
{
  TranslationUnit_t *t;
  Data_Get_Struct(self, TranslationUnit_t, t);
  File_t *f;
  Data_Get_Struct(file, File_t, f);

  Module_t *m;
  VALUE module;
  R_GET_CLASS_DATA("Clangc", "Module", module, Module_t, m);
  m->data = clang_getModuleForFile(t->data, f->data);
  m->parent = self;
  
  if (m->data)
    return module;
  else
    return Qnil;
}
