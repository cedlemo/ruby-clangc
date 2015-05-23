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
/*Index ruby class*/
#include "class_Index.h"
#include "class_TranslationUnit.h"
#include "stdio.h"
#include "macros.h"

static void
c_Index_struct_free(Index_t *s)
{
  if(s)
  {
    
    if(s->data)
    {
      clang_disposeIndex(s->data); 
    }
    ruby_xfree(s);
   }
}  
VALUE
c_Index_struct_alloc(VALUE klass)
{
  Index_t *i;
  i = (Index_t *) ruby_xmalloc(sizeof(Index_t));
  i->data = NULL;
  return Data_Wrap_Struct(klass, NULL, c_Index_struct_free,(void *) i );
}

/*
* call-seq:
*   Clangc::Index.new(exclude_decl_from_pch, display_diagnostics) => Clangc::Index
*
* Provides a shared context for creating translation units.
*
* It provides two options:
*
* - excludeDeclarationsFromPCH:
* When true, allows enumeration of "local"
* declarations (when loading any new translation units). A "local" declaration
* is one that belongs in the translation unit itself and not in a precompiled
* header that was used by the translation unit. If false, all declarations
* will be enumerated.
* The process of creating the 'pch', loading it separately, and using it (via
* -include-pch) allows 'excludeDeclsFromPCH' to remove redundant callbacks
* (which gives the indexer the same performance benefit as the compiler).
*
* - displayDiagnostics:
* When true, default diagnostics are displayed, when false, it is up to the user
* to display them.
*/
VALUE
c_Index_initialize(VALUE self, VALUE excl_decls_from_PCH, VALUE display_diagnostics) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  uint e,d;
  RBOOL_2_INT(excl_decls_from_PCH, e);
  RBOOL_2_INT(display_diagnostics, d);  
  i->data = clang_createIndex( e, d);
  return self;
}
/*
* call-seq:
*   Clangc::Index#global_options=(options) => nil
*
* Sets general options associated with an Index instance.
* - options:
* A bitmask of options, a bitwise OR of the Clangc::GlobalOptFlags constants.
*/
VALUE
c_Index_set_global_options(VALUE self, VALUE options) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  uint c_options;
  RNUM_2_UINT(options, c_options);
  clang_CXIndex_setGlobalOptions(i->data,c_options);
  return Qnil;
}
/*
* call-seq:
*   Clangc::Index#global_options() => num 
*
* Gets the general options associated with an Index.
* A bitmask of options, a bitwise OR of the Clangc::GlobalOptFlags constants.
*/
VALUE
c_Index_get_global_options(VALUE self) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);

  return CUINT_2_NUM(clang_CXIndex_getGlobalOptions(i->data));
}
/*
* call-seq:
*   Clangc::Index#create_translation_unit_from_source_file(source, args) => Clangc::TranslationUnit
*
* Return a TranslationUnit instance for a given source file and the provided
* command line arguments one would pass to the compiler.
*
* - source:
* The source filename argument must be a string but is optional.  If the caller
* provides a nil value, the name of the source file is expected to reside in the
* specified command line arguments.
* - args:
* Must be an array of strings or empty.
* The command-line arguments that would be passed to the clang executable if it
* were being invoked out-of-process.
* These command-line options will be parsed and will affect how the translation
* unit is parsed. Note that the following options are ignored: '-c',
* '-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.
* 
* TODO : (not implemented yet)
* - num_unsaved_files
* the number of unsaved file entries in unsaved_files.
*
* - unsaved_files 
* the files that have not yet been saved to disk but may be required
* for code completion, including the contents of those files.  The contents and name
* of these files (as specified by CXUnsavedFile) are copied when necessary, so the 
* client only needs to guarantee their validity until the call to this function 
* returns.
*/

VALUE
c_Index_create_TU_from_source_file(VALUE self, VALUE source_file, VALUE args) {
  char *c_source_file;
  if(TYPE(source_file == T_STRING))
    c_source_file = StringValueCStr(source_file);
  else
    c_source_file = NULL;

  RARRAY_OF_STRINGS_2_C(args);
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  VALUE tu;
  TranslationUnit_t *c_tu;
  R_GET_CLASS_DATA("Clangc", "TranslationUnit", tu, TranslationUnit_t, c_tu);
  c_tu->data = clang_createTranslationUnitFromSourceFile( i->data,
                                                          c_source_file,
                                                          len, c_args, 0, 0); // TODO manage unsaved files
  c_tu->index = self;
  rb_gc_register_mark_object(self);

  if(c_tu->data != NULL)
    return tu;
  else
    return Qnil;
}
/*
* call-seq:
*   Clangc::Index#create_translation_unit(ast_file) => Clangc::TranslationUnit
*
* Create a translation unit from an AST file name. If the creation fail, it returns
* nil.
* The AST file is created by clang with the option -emit-ast
*/
VALUE
c_Index_create_TU(VALUE self, VALUE ast_file) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  VALUE tu;
  TranslationUnit_t *c_tu;
  R_GET_CLASS_DATA("Clangc", "TranslationUnit", tu, TranslationUnit_t, c_tu);
  char *c_ast_file;
//  if(TYPE(source_file == T_STRING))
  c_ast_file = StringValueCStr(ast_file);
  c_tu->data = clang_createTranslationUnit( i->data, c_ast_file);

  c_tu->index = self;
  rb_gc_register_mark_object(self);

  if(c_tu->data)
    return tu;
  else
    return Qnil;
}
/*
* call-seq:
*   Clangc::Index#create_translation_unit2(ast_file) => Clangc::TranslationUnit or an error code
*
* Create a translation unit from an AST file name. If the creation fail, it returns
* an error code Clangc::ErrorCode. With this implementation, Clangc::ErrorCode::Success is not
* used.
* The AST file is created by clang with the option -emit-ast
*/
VALUE
c_Index_create_TU2(VALUE self, VALUE ast_file) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  VALUE tu;
  TranslationUnit_t *c_tu;
  R_GET_CLASS_DATA("Clangc", "TranslationUnit", tu, TranslationUnit_t, c_tu);
  char *c_ast_file;
//  if(TYPE(source_file == T_STRING))
  c_ast_file = StringValueCStr(ast_file);
  uint er = clang_createTranslationUnit2( i->data, c_ast_file, &(c_tu->data));

  c_tu->index = self;
  rb_gc_register_mark_object(self);

  if(er != 0)
    return CUINT_2_NUM(er);
  else
    return tu;
}
/*
* call-seq:
*   Clangc::Index#parse_translation_unit(source_file, args, options) => Clangc::TranslationUnit
*
* Parse the given source file and generate the translation unit corresponding
* to that file.
*
* This routine is the main entry point for the Clang C API, providing the
* ability to parse a source file into a translation unit that can then be
* queried by other functions in the API. This routine accepts a set of
* command-line arguments so that the compilation can be configured in the same
* way that the compiler is configured on the command line.
*
* - source_file:
* The name of the source file to load, or nil if the source file is included in
* the command line arguments.
*
* - args:
* The command-line arguments that would be passed to the clang executable if it
* were being invoked out-of-process.
* These command-line options will be parsed and will affect how the translation
* unit is parsed. Note that the following options are ignored: '-c', 
* '-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.
*
* - options:
* A bitmask of options that affects how the translation unit is managed but not
* its compilation. This should be a bitwise OR of the TranslationUnit_Flags constants.
*
* TODO:
* - unsaved_files:
* The files that have not yet been saved to disk but may be required for parsing,
* including the contents of those files.  The contents and name of these files
* (as specified by CXUnsavedFile) are copied when necessary, so the client only
* needs to guarantee their validity until the call to this function returns.
*
* - num_unsaved_files:
* The number of unsaved file entries in unsaved_files.
*/
VALUE
c_Index_parse_TU(VALUE self, VALUE source_file, VALUE args, VALUE options) {
  char *c_source_file;
  if(TYPE(source_file == T_STRING))
    c_source_file = StringValueCStr(source_file);
  else
    c_source_file = NULL;
  
  uint c_options;
  RNUM_2_UINT(options, c_options);

  RARRAY_OF_STRINGS_2_C(args);
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  VALUE tu;
  TranslationUnit_t *c_tu;
  R_GET_CLASS_DATA("Clangc", "TranslationUnit", tu, TranslationUnit_t, c_tu);

  c_tu->data = clang_parseTranslationUnit( i->data,
                                                          c_source_file,
                                                          c_args, len, 
                                                          0, 0, c_options); // TODO manage unsaved files
  

  c_tu->index = self;
  rb_gc_register_mark_object(self);

  if (c_tu->data)
    return tu;
  else
    return Qnil;
}
/*
* call-seq:
*   Clangc::Index#parse_translation_unit2(source_file, args, options) => Clangc::TranslationUnit
*
* Parse the given source file and generate the translation unit corresponding
* to that file. If its fails, it returns an Integer corresponding to the 
* error code a Clangc::ErrorCode constant.
*
* This routine is the main entry point for the Clang C API, providing the
* ability to parse a source file into a translation unit that can then be
* queried by other functions in the API. This routine accepts a set of
* command-line arguments so that the compilation can be configured in the same
* way that the compiler is configured on the command line.
*
* - source_file:
* The name of the source file to load, or nil if the source file is included in
* the command line arguments.
*
* - args:
* The command-line arguments that would be passed to the clang executable if it
* were being invoked out-of-process.
* These command-line options will be parsed and will affect how the translation
* unit is parsed. Note that the following options are ignored: '-c', 
* '-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.
*
* - options:
* A bitmask of options that affects how the translation unit is managed but not
* its compilation. This should be a bitwise OR of the TranslationUnit_Flags constants.
*
* TODO:
* - unsaved_files:
* The files that have not yet been saved to disk but may be required for parsing,
* including the contents of those files.  The contents and name of these files
* (as specified by CXUnsavedFile) are copied when necessary, so the client only
* needs to guarantee their validity until the call to this function returns.
*
* - num_unsaved_files:
* The number of unsaved file entries in unsaved_files.
*/
VALUE
c_Index_parse_TU2(VALUE self, VALUE source_file, VALUE args, VALUE options) {
  char *c_source_file;
  if(TYPE(source_file == T_STRING))
    c_source_file = StringValueCStr(source_file);
  else
    c_source_file = NULL;
  
  uint c_options;
  RNUM_2_UINT(options, c_options);

  RARRAY_OF_STRINGS_2_C(args);
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  VALUE tu;
  TranslationUnit_t *c_tu;
  R_GET_CLASS_DATA("Clangc", "TranslationUnit", tu, TranslationUnit_t, c_tu);

  uint er = clang_parseTranslationUnit2(i->data,
                                        c_source_file,
                                        c_args, len, 
                                        0, 0, c_options, // TODO manage unsaved files
                                        &(c_tu->data)); 

  c_tu->index = self;
  rb_gc_register_mark_object(self);

  if(er != 0)
    return CUINT_2_NUM(er);
  else
    return tu;
}
