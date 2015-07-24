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
* the given \p cursor. For many declarations, the lexical and semantic parents
* are equivalent (the lexical parent is returned by 
* \c clang_getCursorLexicalParent()). They diverge when declarations or
* definitions are provided out-of-line. For example:
*
* \code
* class C {
*  void f();
* };
*
* void C::f() { }
* \endcode
*
* In the out-of-line definition of \c C::f, the semantic parent is
* the class \c C, of which this function is a member. The lexical parent is
* the place where the declaration actually occurs in the source code; in this
* case, the definition occurs in the translation unit. In general, the
* lexical parent for a given entity can change without affecting the semantics
* of the program, and the lexical parent of different declarations of the
* same entity may be different. Changing the semantic parent of a declaration,
* on the other hand, can have a major impact on semantics, and redeclarations
* of a particular entity should all have the same semantic context.
*
* In the example above, both declarations of \c C::f have \c C as their
* semantic context, while the lexical context of the first \c C::f is \c C
* and the lexical context of the second \c C::f is the translation unit.
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
