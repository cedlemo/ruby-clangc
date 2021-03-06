/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016 Cedric Le Moigne cedlemo <cedlemo@gmx.com>
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
/*CompletionString ruby class*/
#include "class_CompletionString.h"
#include "macros.h"

static void c_CompletionString_struct_free(CompletionString_t *s)
{
    if (s)
    {
        ruby_xfree(s);
    }
}

static void c_CompletionString_mark(void *s)
{
    if (s)
    {
        CompletionString_t *t = (CompletionString_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_CompletionString_struct_alloc(VALUE klass)
{

    CompletionString_t *ptr;
    ptr = (CompletionString_t *) ruby_xmalloc(sizeof(CompletionString_t));
    ptr->data = NULL;
    ptr->parent = Qnil;

    return Data_Wrap_Struct(
        klass, NULL, c_CompletionString_struct_free, (void *) ptr);
}

/**
 * Clangc::CompletionString#availability => Clangc::AvailabilityKind
 *
 * Determine the availability of the entity that this code-completion
 * string refers to.
 *
 * The availability of the completion string which is one of the
 * Clangc::AvailabilityKind constants.
 */
VALUE
c_CompletionString_get_availability(VALUE self)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CUINT_2_NUM(clang_getCompletionAvailability(c->data));
}

/**
 * call-seq:
 *  Clangc::CompletionString#priority => Number
 *
 * Determine the priority of this code completion.
 *
 * The priority of a code completion indicates how likely it is that this
 * particular completion is the completion that the user will select. The
 * priority is selected by various internal heuristics.
 *
 * Rreturns The priority of this completion string. Smaller values indicate
 * higher-priority (more likely) completions.
 */
VALUE
c_CompletionString_get_priority(VALUE self)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CUINT_2_NUM(clang_getCompletionPriority(c->data));
}

/**
 * call-seq:
 *  Clangc::CompletionString#num_chunks => Number
 *
 * Retrieve the number of chunks in the given code-completion string.
 */
VALUE
c_CompletionString_get_num_chunks(VALUE self)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CUINT_2_NUM(clang_getNumCompletionChunks(c->data));
}

/**
 * call-seq:
 *  Clangc::CompletionString#chunk_kind(Number) => Clangc::CompletionChunkKind
 *
 * Determine the kind of a particular chunk within a completion string.
 *
 * chunk_number the 0-based index of the chunk in the completion string.
 *
 * It returns the kind of the chunk at the index chunk_number (a
 * Clangc::CompletionChunkKind constant).
 */
VALUE
c_CompletionString_get_chunk_kind(VALUE self, VALUE index)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CUINT_2_NUM(clang_getCompletionChunkKind(c->data, NUM2UINT(index)));
}

/**
 * call-seq
 *  Clangc::CompletionString#chunk_text(Number) => String
 *
 * Retrieve the text associated with a particular chunk within a
 * completion string.
 *
 * chunk_number the 0-based index of the chunk in the completion string.
 *
 * Returns the text associated with the chunk at index chunk_number.
 */
VALUE
c_CompletionString_get_chunk_text(VALUE self, VALUE index)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CXSTR_2_RVAL(clang_getCompletionChunkText(c->data, NUM2UINT(index)));
}

/**
 * call-seq:
 *  Clangc::CompletionString#num_annotations => Number
 *
 * Retrieve the number of annotations associated with the given
 * completion string.
 *
 * Returns the number of annotations associated with the given completion
 * string.
 */
VALUE
c_CompletionString_get_num_annotations(VALUE self)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CUINT_2_NUM(clang_getCompletionNumAnnotations(c->data));
}

/**
 * call-seq:
 *  Clangc::CompletionString#annotation(Number)  => String
 *
 * Retrieve the annotation associated with the given completion string.
 *
 * annotation_number the 0-based index of the annotation of the
 * completion string.
 *
 * Returns annotation string associated with the completion at index
 * annotation_number, or a NULL string if that annotation is not available.
 */
VALUE
c_CompletionString_get_annotation(VALUE self, VALUE index)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CXSTR_2_RVAL(
        clang_getCompletionAnnotation(c->data, NUM2UINT(index)));
}

/**
 * Call-seq:
 *  Clangc::CompletionString#brief_comment => String
 *
 * Retrieve the brief documentation comment attached to the declaration
 * that corresponds to the given completion string.
 */
VALUE
c_CompletionString_get_brief_comment(VALUE self)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    return CXSTR_2_RVAL(clang_getCompletionBriefComment(c->data));
}

/**
 * call-seq:
 *  Clangc::CompletionString#chunk_completion_string(index) =>
 * Clangc::CompletionString
 *
 * Retrieve the completion string associated with a particular chunk
 * within a completion string.
 *
 * 0-based index of the chunk in the completion string.
 *
 * Returns the completion string associated with the chunk at index
 * \c chunk_number.
 */
VALUE
c_CompletionString_get_chunk_completion_string(VALUE self, VALUE index)
{
    CompletionString_t *c;
    Data_Get_Struct(self, CompletionString_t, c);

    VALUE completion_string;
    CompletionString_t *cs;
    R_GET_CLASS_DATA("Clangc", CompletionString, completion_string, cs);
    cs->data =
        clang_getCompletionChunkCompletionString(c->data, NUM2UINT(index));
    cs->parent = c->parent;

    return completion_string;
}
