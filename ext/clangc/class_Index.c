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
#include "clang-c/Index.h"
#include "class_Index.h"
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
VALUE
c_Index_set_global_options(VALUE self, VALUE options) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);
  uint c_options;
  RNUM_2_UINT(options, c_options);
  clang_CXIndex_setGlobalOptions(i->data,c_options);
  return Qnil;
}
VALUE
c_Index_get_global_options(VALUE self) {
  Index_t *i;
  Data_Get_Struct(self, Index_t, i);

  return UINT2NUM(clang_CXIndex_getGlobalOptions(i->data));
}
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
  if(c_tu->data != NULL)
    return tu;
  else
    return Qnil;
}
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
  if(c_tu->data)
    return tu;
  else
    return Qnil;
}
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
  
  if (c_tu->data)
    return tu;
  else
    return Qnil;
}

//VALUE
//generate_Index_under(VALUE module, VALUE superclass) {
/*
 An "index" consists of a set of translation units that would
 typically be linked together into an executable or library.
*/
/*  VALUE klass = rb_define_class_under(module, "Index", superclass);
  rb_define_alloc_func(klass, c_Index_struct_alloc);
  rb_define_private_method(klass, "initialize", RUBY_METHOD_FUNC(c_Index_initialize), 2);
  rb_define_method(klass, "global_options=", RUBY_METHOD_FUNC(c_Index_set_global_options), 1);
  rb_define_method(klass, "global_options", RUBY_METHOD_FUNC(c_Index_get_global_options), 0);
  rb_define_method(klass, "create_translation_unit_from_source_file", RUBY_METHOD_FUNC(c_Index_create_TU_from_source_file), 2);
  rb_define_method(klass, "create_translation_unit", RUBY_METHOD_FUNC(c_Index_create_TU), 1);
  rb_define_method(klass, "parse_translation_unit", RUBY_METHOD_FUNC(c_Index_parse_TU), 3);
  return klass;
}*/
