/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2016 Cédric Le Moigne <cedlemo@gmx.com>
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
/*CodeCompleteResults ruby class*/
#include "class_CodeCompleteResults.h"
#include "class_CompletionResult.h"
#include "class_Diagnostic.h"
#include "macros.h"

static void c_CodeCompleteResults_struct_free(CodeCompleteResults_t *s)
{
    if (s)
    {
        if (s->data) clang_disposeCodeCompleteResults(s->data);
        ruby_xfree(s);
    }
}

static void c_CodeCompleteResults_mark(void *s)
{
    if (s)
    {
        CodeCompleteResults_t *t = (CodeCompleteResults_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_CodeCompleteResults_struct_alloc(VALUE klass)
{

    CodeCompleteResults_t *ptr;
    ptr = (CodeCompleteResults_t *) ruby_xmalloc(sizeof(CodeCompleteResults_t));
    ptr->data = NULL;
    ptr->parent = Qnil;

    return Data_Wrap_Struct(
        klass, NULL, c_CodeCompleteResults_struct_free, (void *) ptr);
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#num_results => Number
 *
 * Retrieve the number of Clangc::CompletionResult
 *
 * Not based on libclang function
 */
VALUE
c_CodeCompleteResults_get_num_results(VALUE self)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);

    return CUINT_2_NUM(c->data->NumResults);
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#results(index) => Clangc::CompletionResult
 *
 * Retrieve Clangc::CompletionResult instance a index
 *
 * Return nil if  index < 0 or index >= num results
 * Not based on libclang function
 */
VALUE
c_CodeCompleteResults_get_result(VALUE self, VALUE index)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);
    unsigned i = NUM2UINT(index);

    if (i < 0 || i > c->data->NumResults) return Qnil;

    VALUE result;
    CompletionResult_t *cr;
    R_GET_CLASS_DATA("Clangc", CompletionResult, result, cr);
    cr->data = &(c->data->Results[i]);
    cr->parent = self;

    return result;
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#container_usr => String
 *
 * Returns the USR for the container for the current code completion
 * context. If there is not a container for the current context, this
 * function will return the empty string.
 */
VALUE
c_CodeCompleteResults_get_container_usr(VALUE self)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);

    return CXSTR_2_RVAL(clang_codeCompleteGetContainerUSR(c->data));
}
/**
 * call-seq:
 *  Clangc::CodeCompleteResults#num_diagnostics => Number
 *
 * Determine the number of diagnostics produced prior to the
 * location where code completion was performed.
 */
VALUE
c_CodeCompleteResults_get_num_diagnostics(VALUE self)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);

    return CUINT_2_NUM(clang_codeCompleteGetNumDiagnostics(c->data));
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#diagnostic(index) => Clangc::Diagnostic
 *
 * Retrieve a diagnostic associated with the given code completion.
 */
VALUE
c_CodeCompleteResults_get_diagnostic(VALUE self, VALUE index)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);

    VALUE diagnostic;
    Diagnostic_t *d;
    R_GET_CLASS_DATA("Clangc", Diagnostic, diagnostic, d);

    d->data = clang_codeCompleteGetDiagnostic(c->data, NUM2UINT(index));
    d->parent = c->parent;

    return diagnostic;
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#sort_results => nil
 *
 * Sort the code-completion results in case-insensitive alphabetical
 * order.
 */
VALUE
c_CodeCompleteResults_sort_results(VALUE self)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);

    clang_sortCodeCompletionResults(c->data->Results, c->data->NumResults);
    return Qnil;
}

/**
 * call-seq:
 *  Clangc::CodeCompleteResults#contexts => Number
 *
 * Determines what completions are appropriate for the context
 * the given code completion.
 * 
 * the kinds of completions that are appropriate for use
 * along with the given code completion results.
 */
VALUE
c_CodeCompleteResults_get_contexts(VALUE self)
{
    CodeCompleteResults_t *c;
    Data_Get_Struct(self, CodeCompleteResults_t, c);
    CULLONG_2_NUM(clang_codeCompleteGetContexts(c->data));
}
