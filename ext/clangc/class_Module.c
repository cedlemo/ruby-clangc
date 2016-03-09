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
/*Module ruby class*/
#include "class_Module.h"
#include "class_File.h"
#include "class_TranslationUnit.h"
#include "macros.h"

static void c_Module_struct_free(Module_t *s)
{
    if (s)
    {

        ruby_xfree(s);
    }
}

static void c_Module_mark(void *s)
{
    if (s)
    {
        Module_t *t = (Module_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_Module_struct_alloc(VALUE klass)
{

    Module_t *ptr;
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
    R_GET_CLASS_DATA("Clangc", File, ast_file, f);
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
    R_GET_CLASS_DATA("Clangc", Module, parent, p);
    p->data = clang_Module_getParent(m->data);
    return parent;
}

/**
 * call-seq:
 *  Clangc::Module#name => String
 *
 * Get the name of the module, e.g. for the 'std.vector' sub-module it
 * will return "vector".
 */
VALUE
c_Module_get_name(VALUE self)
{
    Module_t *m;
    Data_Get_Struct(self, Module_t, m);
    return CXSTR_2_RVAL(clang_Module_getName(m->data));
}

/**
 * call-seq:
 *  Clangc::Module#full_name => String
 *
 * Returns the full name of the module, e.g. "std.vector".
 */
VALUE
c_Module_get_full_name(VALUE self)
{
    Module_t *m;
    Data_Get_Struct(self, Module_t, m);
    return CXSTR_2_RVAL(clang_Module_getFullName(m->data));
}

/**
 * call-seq:
 *  Clangc::Module#is_system => true/false
 *
 * Returns non-zero if the module is a system one.
 */
VALUE
c_Module_is_system(VALUE self)
{
    Module_t *m;
    Data_Get_Struct(self, Module_t, m);
    return NOT_0_2_RVAL(clang_Module_isSystem(m->data));
}

/**
 * call-seq:
 *  Clangc::Module#num_top_level_headers(Clangc::TranslationUnit) => Integer
 *
 * The number of top level headers associated with this module.
 */
VALUE
c_Module_get_num_top_level_headers(VALUE self, VALUE translation_unit)
{
    Module_t *m;
    Data_Get_Struct(self, Module_t, m);
    TranslationUnit_t *t;
    CHECK_ARG_TYPE(translation_unit, TranslationUnit);
    Data_Get_Struct(translation_unit, TranslationUnit_t, t);

    return CUINT_2_NUM(clang_Module_getNumTopLevelHeaders(t->data, m->data));
}

/**
 * call-seq:
 *  Clangc::Module#top_level_header(Clangc::TranslationUnit, Integer) =>
 * Clangc::File
 *
 * Index top level header index (zero-based).
 *
 * Returns the specified top level header associated with the module.
 */
VALUE
c_Module_get_top_level_header(VALUE self, VALUE translation_unit, VALUE index)
{
    Module_t *m;
    Data_Get_Struct(self, Module_t, m);
    TranslationUnit_t *t;
    CHECK_ARG_TYPE(translation_unit, TranslationUnit);
    Data_Get_Struct(translation_unit, TranslationUnit_t, t);
    unsigned int c_index = NUM2UINT(index);
    VALUE header;
    File_t *f;
    R_GET_CLASS_DATA("Clangc", File, header, f);
    f->data = clang_Module_getTopLevelHeader(t->data, m->data, c_index);
    if (f->data)
        return header;
    else
        return Qnil;
}
