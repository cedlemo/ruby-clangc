
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
/*Diagnostic ruby class*/
#include "clang-c/Index.h"
#include "class_Diagnostic.h"
#include "macros.h"

static void
c_Diagnostic_struct_free(Diagnostic_t *s)
{
  if(s)
  {
    
  if(s->data)
    clang_disposeDiagnostic(s->data); 

    ruby_xfree(s);
  }
}  
static void
c_Diagnostic_mark(void *s)
{
  if(s)
  {
    Diagnostic_t *d = (Diagnostic_t *)s;
    rb_gc_mark(d->parent);
  }
}
VALUE
c_Diagnostic_struct_alloc( VALUE klass)
{
  
    Diagnostic_t * ptr;
    ptr = (Diagnostic_t *) ruby_xmalloc(sizeof(Diagnostic_t)); 
    ptr->data = NULL;
    ptr->parent = Qnil;
  return Data_Wrap_Struct(klass, c_Diagnostic_mark, c_Diagnostic_struct_free, (void *) ptr);
}

/**
* call-seq:
*   Clangc::Diagnostic#severity => Fixnum
*
* Determine the severity of the given diagnostic. It returns one of the constants defined
* in Clangc::DiagnosticSeverity.constants
*/
VALUE
c_Diagnostic_get_severity(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  unsigned int severity = clang_getDiagnosticSeverity(d->data);
  return CUINT_2_NUM(severity);
}

/**
* call-seq:
*   Clangc::Diagnostic#spelling => String
*
* Retrieve the text of the given diagnostic.
*/
VALUE
c_Diagnostic_get_spelling(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  CXString str = clang_getDiagnosticSpelling(d->data);
  VALUE spelling = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return spelling;
}

/**
* call-seq:
*   Clangc::Diagnostic#category => Fixnum
* 
* Retrieve the category number for this diagnostic.
*
* Diagnostics can be categorized into groups along with other, related
* diagnostics (e.g., diagnostics under the same warning flag). This routine 
* retrieves the category number for the given diagnostic.
*
* The number of the category that contains this diagnostic, or zero
* if this diagnostic is uncategorized.
*/
VALUE
c_Diagnostic_get_category(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  unsigned int category = clang_getDiagnosticCategory(d->data);
  return CUINT_2_NUM(category);
}

/**
* call-seq:
*   Clangc::Diagnostic#category_name => String
*
* Retrieve the name of a particular diagnostic category.  This
* is now deprecated.  Use Clangc::Diagnostic#category_text 
* instead.
*/
VALUE
c_Diagnostic_get_category_name(VALUE self)
{
// TODO deprecated write if macro based on clang vervion
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  CXString str = clang_getDiagnosticCategoryName(clang_getDiagnosticCategory(d->data));
  VALUE name = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return name;
}

/**
* call-seq:
*   Clangc::Diagnostic#category_text => String
*
* Retrieve the diagnostic category text for a given diagnostic.
* Returns The text of the given diagnostic category.
*/
VALUE
c_Diagnostic_get_category_text(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  CXString str = clang_getDiagnosticCategoryText(d->data);
  VALUE text = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return text;
}

/**
* call-seq:
*   Clangc::Diagnostic#num_ranges => Fixnum
*
* Determine the number of source ranges associated with the given
* diagnostic.
*/
VALUE
c_Diagnostic_get_num_ranges(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  unsigned int num = clang_getDiagnosticNumRanges(d->data);
  return CUINT_2_NUM(num);
}

/**
* call-seq:
*   Clangc::Diagnostic#num_fixits => Fixnum
*
* Determine the number of fix-it hints associated with the given
* diagnostic.
*/
VALUE
c_Diagnostic_get_num_fixits(VALUE self)
{
  Diagnostic_t *d;
  Data_Get_Struct(self, Diagnostic_t, d);
  unsigned int num = clang_getDiagnosticNumFixIts(d->data);
  return CUINT_2_NUM(num);
}
