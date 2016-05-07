/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015-2016 Cédric Le Moigne cedlemo <cedlemo@gmx.com>
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

#include "_clangc_functions.h"
#include "clang-c/Index.h"
#include "class_SourceRange.h"
#include "class_SourceLocation.h"
#include "class_Cursor.h"

/**
* call-seq:
*   Clangc.version => String
*
* Return a version string, suitable for showing to a user, but not
* intended to be parsed (the format is not guaranteed to be stable).
*/

VALUE
m_clangc_get_version(VALUE self)
{
    return CXSTR_2_RVAL(clang_getClangVersion());
}

/**
* call-seq:
*   Clangc.default_diagnostic_display_options => Fixnum
*
* Retrieve the set of display options most similar to the
* default behavior of the clang compiler.
*
* A set of display options suitable for use with Clangc::Diagnostic#format
*/

VALUE
m_clangc_get_default_diagnostic_display_options(VALUE self)
{
    return CUINT_2_NUM(clang_defaultDiagnosticDisplayOptions());
}

/**
* call-seq:
*   Clangc.default_editing_translation_unit_options => Fixnum
*
* Returns the set of flags that is suitable for parsing a translation
* unit that is being edited. (Clangc::TranslationUnit_Flags constants)
*
* The set of flags returned provide options for
* Clangc::Index#parseTranslationUnit
* to indicate that the translation unit is likely to be reparsed many times,
* either explicitly (via Clangc::TranslationUnit#reparse) or implicitly
* (e.g., by code completion ( Clangc::TranslationUnit#CodeCompleteAt). The
* returned flag
* set contains an unspecified set of optimizations (e.g., the precompiled
* preamble) geared toward improving the performance of these routines. The
* set of optimizations enabled may change from one version to the next.
*/

VALUE
m_clangc_get_default_editing_translation_unit_options(VALUE self)
{
    return CUINT_2_NUM(clang_defaultEditingTranslationUnitOptions());
}

/**
* call-seq:
*   Clangc.default_code_complete_options => Fixnum
*
* Returns a default set of code-completion options that can be
* passed to Clangc::TranslationUnit#codeCompleteAt.
* This set is be bitwise-OR'd constants of the Clangc::CodeComplete_Flags
*/

VALUE
m_clangc_get_default_code_complete_options(VALUE self)
{
    return CUINT_2_NUM(clang_defaultCodeCompleteOptions());
}

/**
* call-seq:
*   Clangc.null_source_range
*
* Retrieve a NULL (invalid) source range
*/

VALUE
m_clangc_get_null_source_range(VALUE self)
{
    SourceRange_t *s;
    VALUE source_range;
    R_GET_CLASS_DATA("Clangc", SourceRange, source_range, s);
    s->data = clang_getNullRange();
    return source_range;
}

/**
* call-seq:
*   Clangc.null_source_location
*
* Retrieve a NULL (invalid) source location.
*/

VALUE
m_clangc_get_null_source_location(VALUE self)
{
    SourceLocation_t *s;
    VALUE source_location;
    R_GET_CLASS_DATA("Clangc", SourceLocation, source_location, s);
    s->data = clang_getNullLocation();
    return source_location;
}

/**
* call-seq:
*   Clangc.null_cursor
*
* Retrieve a NULL cursor which represents no entity
*/

VALUE
m_clangc_get_null_cursor(VALUE self)
{
    Cursor_t *c;
    VALUE cursor;
    R_GET_CLASS_DATA("Clangc", Cursor, cursor, c);
    c->data = clang_getNullCursor();
    return cursor;
}

typedef struct {
  VALUE callback;
  VALUE parent;
  VALUE cursor;
  int status;
} EvalCallbackData;

static VALUE eval_callback_body( VALUE user_data)
{
    VALUE parent, cursor;
    EvalCallbackData * data = (EvalCallbackData *) user_data;
    parent = data->parent;
    cursor = data->cursor;
    
    return rb_funcall(data->callback, rb_intern("call"), 2, cursor, parent);
}

static enum CXChildVisitResult
visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    VALUE callback, mClangc, cCursor;
    VALUE r_cursor, r_parent;
    VALUE r_ret;
    Cursor_t *c;
    Cursor_t *p;
    unsigned ret;

    callback = (VALUE) client_data;

    mClangc = rb_const_get(rb_cObject, rb_intern("Clangc"));
    cCursor = rb_const_get(mClangc, rb_intern("Cursor"));

    r_cursor = rb_class_new_instance(0, NULL, cCursor);
    Data_Get_Struct(r_cursor, Cursor_t, c);
    c->data = cursor;

    r_parent = rb_class_new_instance(0, NULL, cCursor);
    Data_Get_Struct(r_parent, Cursor_t, p);
    p->data = parent;
    
    EvalCallbackData data;
    data.parent = r_parent;
    data.cursor = r_cursor;
    int status = 0;
    data.callback = callback;

    r_ret = rb_protect(eval_callback_body, (VALUE) (&data), &status);
    
    if(status)
    {
      LOG_ERR("An error occurred in your ruby block while visiting the AST");
      rb_jump_tag(status);
    }
    
    if (TYPE(r_ret) == T_FIXNUM)
    {
        ret = NUM2UINT(r_ret);
        if (ret == CXChildVisit_Break || ret == CXChildVisit_Continue ||
            ret == CXChildVisit_Recurse)
            return ret;
        else
            return CXChildVisit_Break;
    }
    else
        return CXChildVisit_Break;
}

/**
* call-seq:
*   Clangc.visit_children_with_proc(cursor, visitor) => Qtrue/ Qfalse
*
* Visit the children of a particular cursor.
*
* This function visits all the direct children of the given cursor,
* invoking the given visitor Proc with the cursors of each
* visited child. The traversal may be recursive, if the visitor returns
* Clangc::ChildVisitResult::Recurse. The traversal may also be ended
* prematurely, if
* the visitor returns Clangc::ChildVisit::Break.
*
* cursor the cursor whose child may be visited. All kinds of
* cursors can be visited, including invalid cursors (which, by
* definition, have no children).
*
* visitor the visitor function that will be invoked for each
* child of cursor.
*
* returns a true if the traversal was terminated
* prematurely by the visitor returning Clangc::ChildVisitiResult::Break.
*
* You should prefer to use :
*
*     Clangc.visit_children(cursor: cursor, visitor: callback)
*/

VALUE
m_clangc_visit_children_with_proc(VALUE self, VALUE cursor, VALUE aproc)
{
    if (rb_class_of(aproc) != rb_cProc) rb_raise(rb_eTypeError, "Need a block");

    VALUE callback = aproc;
    Cursor_t *c;
    unsigned ret_with_break;

    Data_Get_Struct(cursor, Cursor_t, c);
    ret_with_break = clang_visitChildren(c->data,
                                         visitor,
                                         (CXClientData) callback);
    return NOT_0_2_RVAL(ret_with_break);
}

/**
* call-seq:
*   Clangc.visit_children_with_block(cursor) => Qtrue/ Qfalse
*
* Visit the children of a particular cursor.
*
* This function visits all the direct children of the given cursor,
* invoking the block with the cursors of each
* visited child. The traversal may be recursive, if the visitor returns
* Clangc::ChildVisitResult::Recurse. The traversal may also be ended
* prematurely, if
* the visitor returns Clangc::ChildVisit::Break.
*
* cursor the cursor whose child may be visited. All kinds of
* cursors can be visited, including invalid cursors (which, by
* definition, have no children).
*
* visitor the visitor function that will be invoked for each
* child of cursor.
*
* returns a true if the traversal was terminated
* prematurely by the visitor returning Clangc::ChildVisitiResult::Break.
*
* You should prefer to use :
*
*     Clangc.visit_children(cursor: cursor, visitor: callback)
*/
VALUE
m_clangc_visit_children_with_block(VALUE self, VALUE cursor)
{
    if (rb_block_given_p() == 0) rb_raise(rb_eTypeError, "Need a block");

    VALUE callback = rb_block_proc();
    Cursor_t *c;
    unsigned ret_with_break;

    Data_Get_Struct(cursor, Cursor_t, c);
    ret_with_break = clang_visitChildren(c->data,
                                         visitor,
                                         (CXClientData) callback);
    return NOT_0_2_RVAL(ret_with_break);
}

/**
* call-seq:
*   Clangc.range(Clangc::SourceLocation, Clangc::SourceLocation) =>
* Clangc::SourceRange
*
* Retrieve a source range given the beginning and ending source
* locations.
*/
VALUE
m_clangc_get_range(VALUE self, VALUE begin, VALUE end)
{
    CHECK_ARG_TYPE(begin, SourceLocation);
    CHECK_ARG_TYPE(end, SourceLocation);

    SourceLocation_t *b;
    SourceLocation_t *e;
    VALUE range;
    SourceRange_t *s;
    SourceRange_t *p;

    Data_Get_Struct(begin, SourceLocation_t, b);
    Data_Get_Struct(end, SourceLocation_t, e);
    // TODO Should I add a check to see if they come from the same
    // file / TU
    R_GET_CLASS_DATA("Clangc", SourceRange, range, s);

    s->data = clang_getRange(b->data, e->data);

    // We use the parent of the first parameter (no real reason)
    Data_Get_Struct(b->parent, SourceRange_t, p);
    s->parent = p->parent;

    return range;
}
