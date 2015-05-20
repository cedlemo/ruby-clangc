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
/*TranslationUnit ruby class*/
#include "class_TranslationUnit.h"
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
VALUE
c_TranslationUnit_struct_alloc( VALUE klass)
{
  
    TranslationUnit_t * ptr;
    ptr = (TranslationUnit_t *) ruby_xmalloc(sizeof(TranslationUnit_t)); 
    ptr->data = NULL;

  return Data_Wrap_Struct(klass, NULL, c_TranslationUnit_struct_free, (void *) ptr);
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

