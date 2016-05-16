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
/*CompletionResult ruby class*/
#include "class_CompletionResult.h"
#include "class_CompletionString.h"
#include "macros.h"

static void c_CompletionResult_struct_free(CompletionResult_t *s)
{
    if (s)
    {

        ruby_xfree(s);
    }
}

static void c_CompletionResult_mark(void *s)
{
    if (s)
    {
        CompletionResult_t *t = (CompletionResult_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_CompletionResult_struct_alloc(VALUE klass)
{

    CompletionResult_t *ptr;
    ptr = (CompletionResult_t *) ruby_xmalloc(sizeof(CompletionResult_t));
    ptr->data = NULL;
    ptr->parent = Qnil;

    return Data_Wrap_Struct(
        klass, NULL, c_CompletionResult_struct_free, (void *) ptr);
}

/*
 * call-seq:
 *  Clangc::CompletionResult#cursor_kind => Clangc::CursorKind
 *
 * \brief The kind of entity that this completion refers to.
 *
 * The cursor kind will be a macro, keyword, or a declaration (one of the
 * *Decl cursor kinds), describing the entity that the completion is
 * referring to.
 *
 * In the future, we would like to provide a full cursor, to allow
 * the client to extract additional information from declaration.
 *  Get
 * */
VALUE
c_CompletionResult_get_cursor_kind(VALUE self)
{
    CompletionResult_t *c;
    Data_Get_Struct(self, CompletionResult_t, c);

    return CUINT_2_NUM(c->data->CursorKind);
}

/*
 * call-seq:
 *  Clangc::CompletionResult#completion_string => Clangc::CompetionString
 *
 * The code-completion string that describes how to insert this
 * code-completion result into the editing buffer.
 * */
VALUE
c_CompletionResult_get_completion_string(VALUE self)
{
    CompletionResult_t *c;
    Data_Get_Struct(self, CompletionResult_t, c);

    CompletionString_t *cs;
    VALUE completion_string;
    R_GET_CLASS_DATA("Clangc", CompletionString, completion_string, cs);

    cs->data = c->data->CompletionString;
    cs->parent = self;

    return completion_string;
}
