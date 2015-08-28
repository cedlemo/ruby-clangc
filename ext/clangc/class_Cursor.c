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
/*Cursor ruby class*/
#include "class_Cursor.h"
#include "class_Type.h"
#include "class_SourceLocation.h"
#include "class_SourceRange.h"
#include "class_File.h"
#include "macros.h"

static void
c_Cursor_struct_free(Cursor_t *s)
{
  if(s)
  {
    ruby_xfree(s);
  }
}  

static void
c_Cursor_mark(void *s)
{
  if(s)
  {
    Cursor_t *t =(Cursor_t *)s;
    rb_gc_mark(t->parent);
  }
}

VALUE
c_Cursor_struct_alloc( VALUE klass)
{
  
  Cursor_t * ptr;
  ptr = (Cursor_t *) ruby_xmalloc(sizeof(Cursor_t)); 
  ptr->parent = Qnil;

  return Data_Wrap_Struct(klass, NULL, c_Cursor_struct_free, (void *) ptr);
}

/**
* call-seq:
*   Clangc::Cursor#is_null => true /false
*
*   return true is the cursor is a null Cursor or false otherwise
*/
VALUE
c_Cursor_is_null(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_Cursor_isNull(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#is_equal(cursor1) => true/false
*
* Determine whether two cursors are equivalent.
*/
VALUE
c_Cursor_is_equal(VALUE self, VALUE cursor1)
{
  Cursor_t *c;
  Cursor_t *c1;

  Data_Get_Struct(self, Cursor_t, c);
  Data_Get_Struct(cursor1, Cursor_t, c1);

  if(clang_equalCursors(c->data, c1->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#hash => Fixnum
*
* Compute a hash value for the given cursor.
*/
VALUE
c_Cursor_get_hash(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_hashCursor(c->data)); 
}

/**
* call-seq:
*   Clangc::Cursor#kind => Fixnum
*
* Retrieve the kind of the given cursor.
* The value should refer to the constants in
* Clangc::CursorKind
*/
VALUE
c_Cursor_get_kind(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getCursorKind(c->data)); 
}

/**
* call-seq:
*   Clangc::Cursor#linkage => Fixnum
*
* Determine the linkage of the entity referred to by a given cursor.
* The value should refer to the constants in
* Clangc::LinkageKind
*/
VALUE
c_Cursor_get_linkage(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getCursorLinkage(c->data)); 
}

/**
* call-seq:
*   Clangc::Cursor#availability => Fixnum
*
* Determine the availability of the entity that this cursor refers to,
* taking the current target platform into account.
*
* The availability of the cursor.
* The value should refer to the constants in
* Clangc::AvailabilityKind
*/
VALUE
c_Cursor_get_availability(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getCursorAvailability(c->data)); 
}

/**
* call-seq:
*   Clangc::Cursor#language => Fixnum
*
* Determine the "language" of the entity referred to by a given cursor.
* The value should refer to the constants in
* Clangc::LanguageKind
*/
VALUE
c_Cursor_get_language(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getCursorLanguage(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#type => Clangc::Type
*
* Retrieve the type of a CXCursor (if any).
*/
VALUE
c_Cursor_get_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE type;
  R_GET_CLASS_DATA("Clangc", "Type", type, Type_t, t);
  t->data = clang_getCursorType(c->data);
  t->parent = self;
  return type;
}

/**
* call-seq:
*   Clangc::Cursor#semantic_parent => Clangc::Cursor
*
* Determine the semantic parent of the given cursor.
*
* The semantic parent of a cursor is the cursor that semantically contains
* the given cursor. For many declarations, the lexical and semantic parents
* are equivalent (the lexical parent is returned by 
* Clang::Cursor#lexical_parent). They diverge when declarations or
* definitions are provided out-of-line. For example:
*
*     class C {
*      void f();
*     };
*    
*     void C::f() { }
*
* In the out-of-line definition of "C::f", the semantic parent is
* the class "C", of which this function is a member. The lexical parent is
* the place where the declaration actually occurs in the source code; in this
* case, the definition occurs in the translation unit. In general, the
* lexical parent for a given entity can change without affecting the semantics
* of the program, and the lexical parent of different declarations of the
* same entity may be different. Changing the semantic parent of a declaration,
* on the other hand, can have a major impact on semantics, and redeclarations
* of a particular entity should all have the same semantic context.
*
* In the example above, both declarations of "C::f" have "C" as their
* semantic context, while the lexical context of the first "C::f" is "C"
* and the lexical context of the second "C::f" is the translation unit.
*
* For global declarations, the semantic parent is the translation unit.
*/
VALUE
c_Cursor_get_semantic_parent(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *s;
  VALUE sem_par;
  R_GET_CLASS_DATA("Clangc", "Cursor", sem_par, Cursor_t, s);
  s->data = clang_getCursorSemanticParent(c->data);
  s->parent = c->parent;
  return sem_par;
}

/**
* call-seq:
*   Clangc::Cursor#lexical_parent => Clangc::Cursor
*
* Determine the lexical parent of the given cursor.
*
* The lexical parent of a cursor is the cursor in which the given cursor
* was actually written. For many declarations, the lexical and semantic parents
* are equivalent (the semantic parent is returned by 
* Clangc::Cursor#semantic_parent). They diverge when declarations or
* definitions are provided out-of-line. For example:
*
*     class C {
*      void f();
*     };
*    
*     void C::f() { }
*
* In the out-of-line definition of "C::f", the semantic parent is
* the class "C", of which this function is a member. The lexical parent is
* the place where the declaration actually occurs in the source code; in this
* case, the definition occurs in the translation unit. In general, the
* lexical parent for a given entity can change without affecting the semantics
* of the program, and the lexical parent of different declarations of the
* same entity may be different. Changing the semantic parent of a declaration,
* on the other hand, can have a major impact on semantics, and redeclarations
* of a particular entity should all have the same semantic context.
*
* In the example above, both declarations of "C::f" have "C" as their
* semantic context, while the lexical context of the first "C::f" is "C"
* and the lexical context of the second "C::f" is the translation unit.
*
* For declarations written in the global scope, the lexical parent is
* the translation unit.
*/
VALUE
c_Cursor_get_lexical_parent(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *l;
  VALUE lex_par;
  R_GET_CLASS_DATA("Clangc", "Cursor", lex_par, Cursor_t, l);
  l->data = clang_getCursorLexicalParent(c->data);
  l->parent = c->parent;
  return lex_par;
}

/**
* call-seq:
*   Clangc::Cursor#location => Clangc::SourceLocation
*
* Retrieve the physical location of the source constructor referenced
* by the given cursor.
*
* The location of a declaration is typically the location of the name of that
* declaration, where the name of that declaration would occur if it is
* unnamed, or some keyword that introduces that particular declaration.
* The location of a reference is where that reference occurs within the
* source code.
*/
VALUE
c_Cursor_get_source_location(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  SourceLocation_t *s;
  VALUE src_loc;
  R_GET_CLASS_DATA("Clangc", "SourceLocation", src_loc, SourceLocation_t, s);
  s->data = clang_getCursorLocation(c->data);
  s->parent = self;
  return src_loc;
}

/**
* call-seq:
*   Clangc::Cursor#extent => Clangc::SourceRange
*
* Retrieve the physical extent of the source construct referenced by
* the given cursor.
*
* The extent of a cursor starts with the file/line/column pointing at the
* first character within the source construct that the cursor refers to and
* ends with the last character within that source construct. For a
* declaration, the extent covers the declaration itself. For a reference,
* the extent covers the location of the reference (e.g., where the referenced
* entity was actually used).
*/
VALUE
c_Cursor_get_extent(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  SourceRange_t *s;
  VALUE src_rge;
  R_GET_CLASS_DATA("Clangc", "SourceRange", src_rge, SourceRange_t, s);
  s->data = clang_getCursorExtent(c->data);
  s->parent = self;
  return src_rge;
}

/**
* call-seq:
*   Clangc::Cursor#spelling => String
*
* Retrieve a name for the entity referenced by this cursor.
*/
VALUE
c_Cursor_get_spelling(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_getCursorSpelling(c->data);
  VALUE spelling = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return spelling;
}

/**
*   Clangc::Cursor#typedef_decl_underlying_type => Clangc::Type
*
* Retrieve the underlying type of a typedef declaration.
*
* If the cursor does not reference a typedef declaration, an invalid type is
* returned.
*/
VALUE
c_Cursor_get_typedef_decl_underlying_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE type;
  R_GET_CLASS_DATA("Clangc", "Type", type, Type_t, t);
  t->data = clang_getTypedefDeclUnderlyingType(c->data);
  t->parent = c->parent;
  return type;
}

/**
* call-seq:
*   Clangc::Cursor#included_file => Clangc::File
*
* Retrieve the file that is included by the given inclusion directive
* cursor.
*/
VALUE
c_Cursor_get_included_file(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  VALUE included;
  File_t *f;
  R_GET_CLASS_DATA("Clangc", "File", included, File_t, f);
  f->data = clang_getIncludedFile(c->data);
  f->parent = c->parent;
  return included;
}

/**
* call-seq:
*   Clangc::Cursor#is_declaration => true/false
*
* Determine whether the given cursor kind represents a declaration.
*/
VALUE
c_Cursor_is_declaration(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isDeclaration(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_reference => true/false
*
* Determine whether the given cursor kind represents a simple
* reference.
*
* Note that other kinds of cursors (such as expressions) can also refer to
* other cursors. Use clang_getCursorReferenced() to determine whether a
* particular cursor refers to another entity.
*/
VALUE
c_Cursor_is_reference(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isReference(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}
