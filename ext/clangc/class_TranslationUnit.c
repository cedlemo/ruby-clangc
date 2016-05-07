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
#include "class_CodeCompleteResults.h"
#include "macros.h"

static void c_TranslationUnit_struct_free(TranslationUnit_t *s)
{
    if (s)
    {

        if (s->data) clang_disposeTranslationUnit(s->data);

        ruby_xfree(s);
    }
}
static void c_TranslationUnit_mark(void *s)
{
    if (s)
    {
        TranslationUnit_t *t = (TranslationUnit_t *) s;
        rb_gc_mark(t->parent);
    }
}

VALUE
c_TranslationUnit_struct_alloc(VALUE klass)
{

    TranslationUnit_t *ptr;
    ptr = (TranslationUnit_t *) ruby_xmalloc(sizeof(TranslationUnit_t));
    ptr->data = NULL;
    ptr->parent = Qnil;
    return Data_Wrap_Struct(klass,
                            c_TranslationUnit_mark,
                            c_TranslationUnit_struct_free,
                            (void *) ptr);
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
    unsigned int num;
    Data_Get_Struct(self, TranslationUnit_t, t);

    num = clang_getNumDiagnostics(t->data);
    return CUINT_2_NUM(num);
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#default_save_options => num
 *
 * Returns the set of flags that is suitable for saving a translation
 * unit. Those flags should be Clangc::SaveTranslationUnit_Flags constant
 *
 * The set of flags returned provide options for Clangc::TranslationUnit#save by
 * default.
 * The returned flags set contains an unspecified set of options that save
 * translation units with
 * the most commonly-requested data.
 */
VALUE
c_TranslationUnit_get_default_save_options(VALUE self)
{
    TranslationUnit_t *t;
    unsigned int num;
    Data_Get_Struct(self, TranslationUnit_t, t);

    num = clang_defaultSaveOptions(t->data);
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
 * The set of flags returned provide options for Clangc::TranslationUnit.reparse
 * by default. The returned flag set contains an unspecified set of
 * optimizations
 * geared toward common uses of reparsing. The set of optimizations enabled may
 * change from one version  to the next. Clangc::ReparseF_Flags constants.
 */

VALUE
c_TranslationUnit_get_default_reparse_options(VALUE self)
{
    TranslationUnit_t *t;
    unsigned int num;
    Data_Get_Struct(self, TranslationUnit_t, t);

    num = clang_defaultReparseOptions(t->data);
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
    unsigned int max;
    unsigned int c_num;
    VALUE diagnostic;

    max = clang_getNumDiagnostics(t->data);
    c_num = NUM2UINT(num);
    CHECK_IN_RANGE(c_num, 0, max);
    Diagnostic_t *d;
    R_GET_CLASS_DATA("Clangc", Diagnostic, diagnostic, d);
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
    CXFile cxfile;
    char *c_file_name;

    Data_Get_Struct(self, TranslationUnit_t, t);
    c_file_name = RSTRING_2_CHAR(file_name);
    cxfile = clang_getFile(t->data, c_file_name);

    if (cxfile)
    {
        VALUE file;
        File_t *f;
        R_GET_CLASS_DATA("Clangc", File, file, f);
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
    R_GET_CLASS_DATA("Clangc", Cursor, a_cursor, c);
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
    File_t *f;
    Module_t *m;
    VALUE module;

    Data_Get_Struct(self, TranslationUnit_t, t);
    CHECK_ARG_TYPE(file, File);
    Data_Get_Struct(file, File_t, f);

    R_GET_CLASS_DATA("Clangc", Module, module, m);
    m->data = clang_getModuleForFile(t->data, f->data);
    m->parent = self;

    if (m->data)
        return module;
    else
        return Qnil;
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#code_complete_at(filename, line, column, options) =>
 * Clangc::CodeCompleteResults or nil
 *
 * Perform code completion at a given location in a translation unit.
 *
 * This function performs code completion at a particular file, line, and
 * column within source code, providing results that suggest potential
 * code snippets based on the context of the completion. The basic model
 * for code completion is that Clang will parse a complete source file,
 * performing syntax checking up to the location where code-completion has
 * been requested. At that point, a special code-completion token is passed
 * to the parser, which recognizes this token and determines, based on the
 * current location in the C/Objective-C/C++ grammar and the state of
 * semantic analysis, what completions to provide. These completions are
 * returned via a new Clangc::CodeCompleteResults instantce.
 *
 * Code completion itself is meant to be triggered by the client when the
 * user types punctuation characters or whitespace, at which point the
 * code-completion location will coincide with the cursor. For example, if \c p
 * is a pointer, code-completion might be triggered after the "-" and then
 * after the ">" in \c p->. When the code-completion location is afer the ">",
 * the completion results will provide, e.g., the members of the struct that
 * "p" points to. The client is responsible for placing the cursor at the
 * beginning of the token currently being typed, then filtering the results
 * based on the contents of the token. For example, when code-completing for
 * the expression \c p->get, the client should provide the location just after
 * the ">" (e.g., pointing at the "g") to this code-completion hook. Then, the
 * client can filter the results based on the current token text ("get"), only
 * showing those results that start with "get". The intent of this interface
 * is to separate the relatively high-latency acquisition of code-completion
 * results from the filtering of results on a per-character basis, which must
 * have a lower latency.
 *
 * Self is The translation unit in which code-completion should
 * occur. The source files for this translation unit need not be
 * completely up-to-date (and the contents of those source files may
 * be overridden via \p unsaved_files). Cursors referring into the
 * translation unit may be invalidated by this invocation.
 *
 * The name of the source file where code
 * completion should be performed. This filename may be any file
 * included in the translation unit.
 *
 * The line at which code-completion should occur.
 *
 * The column at which code-completion should occur.
 * Note that the column should point just after the syntactic construct that
 * initiated code completion, and not in the middle of a lexical token.
 *
 * # TODO unsaved_files (not managed yet) the Tiles that have not yet been saved
 * to disk
 * but may be required for parsing or code completion, including the
 * contents of those files.  The contents and name of these files (as
 * specified by CXUnsavedFile) are copied when necessary, so the
 * client only needs to guarantee their validity until the call to
 * this function returns.
 *
 * # TODO num_unsaved_files The number of unsaved file entries in \p
 * unsaved_files.
 *
 * Extra options that control the behavior of code
 * completion, expressed as a bitwise OR of the constants of the
 * Clangc::CodeComplete_Flags. The
 * Clangc_default_code_complete_options function returns a default set
 * of code-completion options.
 *
 * \returns If successful, a new \c CXCodeCompleteResults structure
 * containing code-completion results, which should eventually be
 * freed with \c clang_disposeCodeCompleteResults(). If code
 * completion fails, returns NULL.
 */
VALUE
c_TranslationUnit_code_complete_at(
    VALUE self, VALUE filename, VALUE line, VALUE column, VALUE options)
{
    TranslationUnit_t *t;
    CodeCompleteResults_t *c;
    VALUE code_complete_results;
    char *c_filename;
    unsigned int c_line;
    unsigned int c_column;
    unsigned int c_options;

    c_filename = RSTRING_2_CHAR(filename);
    c_line = NUM2UINT(line);
    c_column = NUM2UINT(column);
    c_options = CLANGC_CONSTANT_TO_UINT("CodeComplete_Flags", options);

    Data_Get_Struct(self, TranslationUnit_t, t);
    R_GET_CLASS_DATA("Clangc", CodeCompleteResults, code_complete_results, c);
    c->data = clang_codeCompleteAt(t->data,
                                   c_filename,
                                   c_line,
                                   c_column,
                                   NULL, // TODO Manage unsaved files
                                   0,
                                   c_options);
    c->parent = self;

    if (c->data)
        return code_complete_results;
    else
        return Qnil;
}

/**
 * call-seq:
 *  Clangc::TranslationUnit#reparse(options) => Clangc::ErrorCode
 *
 * Reparse the source files that produced this translation unit.
 *
 * This routine can be used to re-parse the source files that originally
 * created the given translation unit, for example because those source files
 * have changed (either on disk or as passed via \p unsaved_files). The
 * source code will be reparsed with the same command-line options as it
 * was originally parsed.
 *
 * Reparsing a translation unit invalidates all cursors and source locations
 * that refer into that translation unit. This makes reparsing a translation
 * unit semantically equivalent to destroying the translation unit and then
 * creating a new translation unit with the same command-line arguments.
 * However, it may be more efficient to reparse a translation
 * unit using this routine.
 *
 * The translation unit whose contents will be re-parsed. The
 * translation unit must originally have been built with
 * \c clang_createTranslationUnitFromSourceFile().
 *
 * TODO  num_unsaved_files The number of unsaved file entries in \p
 * unsaved_files.
 *
 * TODO unsaved_files The files that have not yet been saved to disk
 * but may be required for parsing, including the contents of
 * those files.  The contents and name of these files (as specified by
 * CXUnsavedFile) are copied when necessary, so the client only needs to
 * guarantee their validity until the call to this function returns.
 *
 * options A bitset of options composed of the flags in Clangc::Reparse_Flags.
 * The function Clangc::TranslationUnit.defaultReparseOptions() produces a
 * default set of
 * options recommended for most uses, based on the translation unit.
 *
 * \returns 0 if the sources could be reparsed.  A non-zero error code will be
 * returned if reparsing was impossible, such that the translation unit is
 * invalid. The error codes returned by this
 * routine are described by the Clangc::ErrorCode.
 */
VALUE
c_TranslationUnit_reparse(VALUE self, VALUE options)
{
    TranslationUnit_t *t;
    int error;
    unsigned int c_options;

    Data_Get_Struct(self, TranslationUnit_t, t);
    c_options = CLANGC_CONSTANT_TO_UINT("Reparse_Flags", options);
    error = clang_reparseTranslationUnit(t->data,
                                         0,
                                         NULL, // TODO Manage unsaved files
                                         c_options);
    return CINT_2_NUM(error);
}
