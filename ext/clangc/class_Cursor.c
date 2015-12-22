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
#include "class_CompletionString.h"
#include "class_TranslationUnit.h"
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
* call-seq:
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
* other cursors. Use Clangc::Cursor#cursor_referenced to determine whether a
* particular cursor refers to another entity.
*/
VALUE
c_Cursor_is_reference(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isReference(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_expression => true/false
*
* Determine whether the given cursor kind represents an expression.
*/
VALUE
c_Cursor_is_expression(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isExpression(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_statement => true/false
*
* Determine whether the given cursor kind represents a statement.
*/
VALUE
c_Cursor_is_statement(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isStatement(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_attribute => true/false
*
* Determine whether the given cursor kind represents an attribute.
*/
VALUE
c_Cursor_is_attribute(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isAttribute(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_invalid => true/false
*
* Determine whether the given cursor kind represents an invalid
* cursor.
*/
VALUE
c_Cursor_is_invalid(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isInvalid(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_translation_unit => true/false
*
* Determine whether the given cursor kind represents a translation
* unit.
*/
VALUE
c_Cursor_is_translation_unit(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isTranslationUnit(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Cursor#is_preprocessing => true/false
*
* Determine whether the given cursor represents a preprocessing
* element, such as a preprocessor directive or macro instantiation.
*/
VALUE
c_Cursor_is_preprocessing(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return clang_isPreprocessing(clang_getCursorKind(c->data)) == 0 ? Qfalse : Qtrue;
}
/**
* call-seq:
*   Clangc::Cursor#enum_integer_type => Clangc::Type
*
* Retrieve the integer type of an enum declaration.
*
* If the cursor does not reference an enum declaration, an invalid type is
* returned.
*/
VALUE
c_Cursor_get_enum_decl_integer_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE type;
  R_GET_CLASS_DATA("Clangc", "Type", type, Type_t, t);
  t->data = clang_getEnumDeclIntegerType(c->data);
  t->parent = c->parent;
  return type;
}

/**
* call-seq:
*   Clangc::Cursor#enum_const_decl_value => Fixnum
*
* Retrieve the integer value of an enum constant declaration as a signed
* long long.
*
* If the cursor does not reference an enum constant declaration, ULLONG_MIN is returned.
* Since this is also potentially a valid constant value, the kind of the cursor
* must be verified before calling this function.
*/
VALUE
c_Cursor_get_enum_const_decl_value(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CLLONG_2_NUM(clang_getEnumConstantDeclValue(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#enum_const_decl_unsigned_value => Fixnum
*
* Retrieve the integer value of an enum constant declaration as an unsigned
* long long.
*
* If the cursor does not reference an enum constant declaration, ULLONG_MAX is returned.
* Since this is also potentially a valid constant value, the kind of the cursor
* must be verified before calling this function.
*/
VALUE
c_Cursor_get_enum_const_decl_unsigned_value(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CULLONG_2_NUM(clang_getEnumConstantDeclUnsignedValue(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#field_decl_bit_width => Fixnum
*
* Retrieve the bit width of a bit field declaration as an integer.
*
* If a cursor that is not a bit field declaration is passed in, -1 is returned.
*/
VALUE
c_Cursor_get_field_decl_bit_width(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CINT_2_NUM(clang_getFieldDeclBitWidth(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#num_arguments => Fixnum
*
* Retrieve the number of non-variadic arguments associated with a given
* cursor.
*
* The number of arguments can be determined for calls as well as for
* declarations of functions or methods. For other cursors -1 is returned.
*/
VALUE
c_Cursor_get_num_arguments(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CINT_2_NUM(clang_Cursor_getNumArguments(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#get_argument(index) => Clangc::Cursor
*
* Retrieve the argument cursor of a function or method.
*
* The argument cursor can be determined for calls as well as for declarations
* of functions or methods. For other cursors and for invalid indices, an
* invalid cursor is returned.
*/
VALUE
c_Cursor_get_argument(VALUE self, VALUE index)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  int max = clang_Cursor_getNumArguments(c->data);
  if(max < 0)
    max = 0;
  unsigned int c_index;
  RNUM_2_UINT(index, c_index);
  CHECK_IN_RANGE(c_index, 0, max);
  Cursor_t *a;
  VALUE arg;
  R_GET_CLASS_DATA("Clangc", "Cursor", arg, Cursor_t, a);
  a->data = clang_Cursor_getArgument(c->data, c_index);
  a->parent = c->parent;
  return arg;
}

/**
* call-seq:
*   Clangc::Cursor#num_template_arguments => Fixnum
*
* Returns the number of template args of a function decl representing a
* template specialization.
*
* If the argument cursor cannot be converted into a template function
* declaration, -1 is returned.
*
* For example, for the following declaration and specialization:

*    template <typename T, int kInt, bool kBool>
*    void foo() { ... }
*    
*    template <>
*    void foo<float, -7, true>();
*
* The value 3 would be returned from this call.
*/
VALUE
c_Cursor_get_num_template_arguments(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CINT_2_NUM(clang_Cursor_getNumTemplateArguments(c->data));  
}
/**
* call-seq:
*   Clangc::Cursor#decl_objectC_type_encoding => string
*
* Returns the Objective-C type encoding for the specified declaration.
*/
VALUE
c_Cursor_get_decl_obj_c_type_encoding(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_getDeclObjCTypeEncoding(c->data);
  VALUE type_encoding = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return type_encoding;
}

/**
* call-seq:
*   Clangc::Cursor#result_tye => Clangc::Type
*
* Retrieve the return type associated with a function type.
*
* If a non-function type is passed in, an invalid type is returned.
*/
VALUE
c_Cursor_get_result_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE result_type;
  R_GET_CLASS_DATA("Clangc", "Type", result_type, Type_t, t);
  t->data = clang_getCursorResultType(c->data);
  t->parent = self;
  return result_type;
}

/**
* call-seq:
*   Clangc::Cursor#offset_field => Fixnum
*
* Return the offset of the field represented by the Cursor.
*
* If the cursor is not a field declaration, -1 is returned.
* If the cursor semantic parent is not a record field declaration,
* Clangc::TypeLayoutError::INVALID is returned.
* If the field's type declaration is an incomplete type,
* Clangc::TypeLayoutError::INCOMPLETE is returned.
* If the field's type declaration is a dependent type,
* Clangc::TypeLayoutError::DEPENDENT is returned.
* If the field's name S is not found,
* Clangc::TypeLayoutError::INVALIDfIELD_NAME is returned.
*/
VALUE
c_Cursor_get_offset_of_field(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CLLONG_2_NUM(clang_Cursor_getOffsetOfField(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#is_anonymous => true / false
*
* Determine whether the given cursor represents an anonymous record
* declaration.
*/
VALUE
c_Cursor_is_anonymous(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  
  if(clang_Cursor_isAnonymous(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#is_bit_field => true/false
*
* Returns true if the cursor specifies a Record member that is a
* bitfield.
*/
VALUE
c_Cursor_is_bit_field(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);

  if(clang_Cursor_isBitField(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#is_virtual_base => true/false
*
* Returns true if the base class specified by the cursor with kind
* Clangc::CursorKind::CXX_BASE_SPECIFIER is virtual.
*/
VALUE
c_Cursor_is_virtual_base(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);

  if(clang_isVirtualBase(c->data) == 1)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#cxx_access_specifier =>  Clangc::CXXAccessSpecifier.constants
*
* Returns the access control level for the referenced object. 
* The returned value is one of the Clangc::CXXAccesSpecifier.constants.
* It represents the C++ access control level to a base class for a
* cursor with kind Clangc::Cursor::CXX_BASE_SPECIFIER.
*
* If the cursor refers to a C++ declaration, its access control level within its
* parent scope is returned. Otherwise, if the cursor refers to a base specifier or
* access specifier, the specifier itself is returned.
*/
VALUE
c_Cursor_get_cxx_access_specifier(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getCXXAccessSpecifier(c->data));  
}
/**
* call-seq:
*   Clangc::Cursor#storage_class => Clangc::StorageClass.constants
*
* Returns the storage class for a function or variable declaration.
*
* If the passed in Cursor is not a function or variable declaration,
* Clangc::StorageClass::SC_INVALID is returned else the storage class.
*/
VALUE
c_Cursor_get_storage_class(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_Cursor_getStorageClass(c->data));  
}

/**
* call-seq:
*   Clangc::Cursor#num_overloaded_decls => Fixnum >= 0
*
* Determine the number of overloaded declarations referenced by a 
* Clangc::CursorKind::OVERLOADED_DECL_REF cursor.
*
* The number of overloaded declarations referenced by cursor. If it
* is not a Clangc::CursorKind::OVERLOADED_DECL_REF cursor, returns 0.
*/
VALUE
c_Cursor_get_num_overloaded_decls(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getNumOverloadedDecls(c->data));  
}

/**
* call-seq:
*   Clangc::Cursor#overloaded_decl(index) => Clangc::Cursor
*
* Retrieve a cursor for one of the overloaded declarations referenced
* by a Clangc::CursorKind::OVERLOADED_DECL_REF cursor.
*
* index The zero-based index into the set of overloaded declarations in
* the cursor.
*
* Returns a cursor representing the declaration referenced by the given 
* cursor at the specified index. If the cursor does not have an 
* associated set of overloaded declarations, or if the index is out of bounds,
* returns a nul cursor;
*/
VALUE
c_Cursor_get_overloaded_decl(VALUE self, VALUE index)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  unsigned int c_index;
  RNUM_2_UINT(index, c_index);
  Cursor_t *o;
  VALUE overl_decl;
  R_GET_CLASS_DATA("Clangc", "Cursor", overl_decl, Cursor_t, o);
  o->data = clang_getOverloadedDecl(c->data, c_index);
  o->parent = c->parent;
  return overl_decl;
}
/**
* call-seq:
*   Clangc::Cursor#display_name => String
*
* Retrieve the display name for the entity referenced by this cursor.
*
* The display name contains extra information that helps identify the cursor,
* such as the parameters of a function or template or the arguments of a 
* class template specialization.
*/
VALUE
c_Cursor_get_display_name(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_getCursorDisplayName(c->data);
  VALUE display_name = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return display_name;
}

/**
* call-seq:
*   Clangc::Cursor#ib_outlet_collection_type => Clangc::Type
*
* For cursors representing an iboutletcollection attribute,
*  this function returns the collection element type.
*/
VALUE
c_Cursor_get_ib_outlet_collection_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE collection_type;
  R_GET_CLASS_DATA("Clangc", "Type", collection_type, Type_t, t);
  t->data = clang_getIBOutletCollectionType(c->data);
  t->parent = self;
  return collection_type;
}

/**
* call-seq:
*   Clangc::Cursor#usr => String
*
* Retrieve a Unified Symbol Resolution (USR) for the entity referenced
* by the given cursor.
*
* A Unified Symbol Resolution (USR) is a string that identifies a particular
* entity (function, class, variable, etc.) within a program. USRs can be
* compared across translation units to determine, e.g., when references in
* one translation refer to an entity defined in another translation unit.
*/
VALUE
c_Cursor_get_usr(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_getCursorUSR(c->data);
  VALUE usr = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return usr;
}
/**
* call-seq:
*   Clangc::Cursor#referenced => Clangc::Cursor
*
* For a cursor that is a reference, retrieve a cursor representing the
* entity that it references.
*
* Reference cursors refer to other entities in the AST. For example, an
* Objective-C superclass reference cursor refers to an Objective-C class.
* This function produces the cursor for the Objective-C class from the
* cursor for the superclass reference. If the input cursor is a declaration or
* definition, it returns that declaration or definition unchanged.
* Otherwise, returns the NULL cursor.
*/
VALUE
c_Cursor_get_referenced(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *r;
  VALUE referenced;
  R_GET_CLASS_DATA("Clangc", "Cursor", referenced, Cursor_t, r);
  r->data = clang_getCursorReferenced(c->data);
  r->parent = c->parent;
  return referenced;
}

/**
* call-seq:
*   Clangc::Cursor#definition => Clangc::Cursor
*
* For a cursor that is either a reference to or a declaration
* of some entity, retrieve a cursor that describes the definition of
* that entity.
*
* Some entities can be declared multiple times within a translation
* unit, but only one of those declarations can also be a
* definition. For example, given:
*
*     int f(int, int);
*     int g(int x, int y) { return f(x, y); }
*     int f(int a, int b) { return a + b; }
*     int f(int, int);
*
* there are three declarations of the function "f", but only the
* second one is a definition. The clang_getCursorDefinition()
* function will take any cursor pointing to a declaration of "f"
* (the first or fourth lines of the example) or a cursor referenced
* that uses "f" (the call to "f' inside "g") and will return a
* declaration cursor pointing to the definition (the second "f"
* declaration).
*
* If given a cursor for which there is no corresponding definition,
* e.g., because there is no definition of that entity within this
* translation unit, returns a NULL cursor.
*/
VALUE
c_Cursor_get_definition(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *d;
  VALUE definition;
  R_GET_CLASS_DATA("Clangc", "Cursor", definition, Cursor_t, d);
  d->data = clang_getCursorDefinition(c->data);
  d->parent = c->parent;
  return definition;
}

/**
* call-seq:
*   Clangc::Cursor#is_definition => true/false
*
* Determine whether the declaration pointed to by this cursor
* is also a definition of that entity.
*/
VALUE
c_Cursor_is_definition(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_isCursorDefinition(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#canonical_cursor => Clangc::Cursor
*
* Retrieve the canonical cursor corresponding to the given cursor.
*
* In the C family of languages, many kinds of entities can be declared several
* times within a single translation unit. For example, a structure type can
* be forward-declared (possibly multiple times) and later defined:
*
*   struct X;
*   struct X;
*   struct X {
*     int member;
*   };
*
* The declarations and the definition of \c X are represented by three 
* different cursors, all of which are declarations of the same underlying 
* entity. One of these cursor is considered the "canonical" cursor, which
* is effectively the representative for the underlying entity. One can 
* determine if two cursors are declarations of the same underlying entity by
* comparing their canonical cursors.
*
* It returns The canonical cursor for the entity referred to by the given cursor.
*/
VALUE
c_Cursor_get_canonical_cursor(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *cc;
  VALUE canonical_cursor;
  R_GET_CLASS_DATA("Clangc", "Cursor", canonical_cursor, Cursor_t, cc);
  cc->data = clang_getCanonicalCursor(c->data);
  cc->parent = c->parent;
  return canonical_cursor;
}
/**
* call-seq:
*   Clangc::Cursor#obj_c_selector_index => Fixnum
*
* If the cursor points to a selector identifier in an Objective-C
* method or message expression, this returns the selector index.
*
* After getting a cursor with #clang_getCursor, this can be called to
* determine if the location points to a selector identifier.
*
* It returns The selector index if the cursor is an Objective-C method or message
* expression and the cursor is pointing to a selector identifier, or -1
* otherwise.
*/
VALUE
c_Cursor_get_obj_c_selector_index(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CINT_2_NUM(clang_Cursor_getObjCSelectorIndex(c->data));
}
/**
* call-seq:
*   Clangc::Cursor#is_dynamic_call => true/ false
*
* Given a cursor pointing to a C++ method call or an Objective-C
* message, returns non-zero if the method/message is "dynamic", meaning:
* 
* For a C++ method: the call is virtual.
* For an Objective-C message: the receiver is an object instance, not 'super'
* or a specific class.
* 
* If the method/message is "static" or the cursor does not point to a
* method/message, it will return zero.
*/
VALUE
c_Cursor_is_dynamic_call(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_Cursor_isDynamicCall(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}
/**
* call-seq:
*   Clangc::Cursor#receiver_type => Clangc::Type
*
* Given a cursor pointing to an Objective-C message, returns the CXType
* of the receiver.
*/
VALUE
c_Cursor_get_receiver_type(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Type_t *t;
  VALUE receiver_type;
  R_GET_CLASS_DATA("Clangc", "Type", receiver_type, Type_t, t);
  t->data = clang_Cursor_getReceiverType(c->data);
  t->parent = self;
  return receiver_type;
}
/**
* call-seq:
*   Clangc::Cursor#obj_c_decl_qualifiers => Fixnum 
 
* \brief Given a cursor that represents an Objective-C method or parameter
* declaration, return the associated Objective-C qualifiers for the return
* type or the parameter respectively. The bits are formed from
* Clangc::ObjCDeclQualifierKind constants.
*/
VALUE
c_Cursor_get_obj_c_decl_qualifiers(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_Cursor_getObjCDeclQualifiers(c->data));
}

/**
* call-seq:
*   Clangc::Cursor#is_obj_c_optional => true / false
*
* Given a cursor that represents an Objective-C method or property
* declaration, return non-zero if the declaration was affected by "@optional".
* Returns zero if the cursor is not such a declaration or it is "@required".
*/
VALUE
c_Cursor_is_obj_c_optional(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_Cursor_isObjCOptional(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}
/**
* call-seq:
*   Clangc::Cursor#is_variadic => true / false
*
* Returns non-zero if the given cursor is a variadic function or method.
*/
VALUE
c_Cursor_is_variadic(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_Cursor_isVariadic(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}
/**
* call-seq:
*   Clangc::Cursor#comment_range => Clangc::SourceRange
*
* Given a cursor that represents a declaration, return the associated
* comment's source range.  The range may include multiple consecutive comments
* with whitespace in between.
*/
VALUE
c_Cursor_get_comment_range(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  SourceRange_t *s;
  VALUE cmt_rge;
  R_GET_CLASS_DATA("Clangc", "SourceRange", cmt_rge, SourceRange_t, s);
  s->data = clang_Cursor_getCommentRange(c->data);
  s->parent = self;
  return cmt_rge;
}
/**
* call-seq:
*   Clangc::Cursor#raw_comment_text => String
*
* Given a cursor that represents a declaration, return the associated
* comment text, including comment markers.
*/
VALUE
c_Cursor_get_raw_comment_text(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_Cursor_getRawCommentText(c->data);
  VALUE raw_comment = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return raw_comment;
}

/**
* call-seq:
*   Clangc::Cursor#brief_comment_text => String
*
* Given a cursor that represents a documentable entity (e.g.,
* declaration), return the associated \\brief paragraph; otherwise return the
* first paragraph.
*/
VALUE
c_Cursor_get_brief_comment_text(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_Cursor_getBriefCommentText(c->data);
  VALUE brief_comment = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return brief_comment;
}
/**
* call-seq:
*  Clangc::Cursor#mangling => String
* Retrieve the String representing the mangled name of the cursor.
*/
VALUE
c_Cursor_get_mangling(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CXString str = clang_Cursor_getMangling(c->data);
  VALUE mangling = rb_str_new2(clang_getCString(str) );
  clang_disposeString(str);
  return mangling;
}

/**
* call-seq:
*   Clangc::Cursor#cxx_method_is_pure_virtual => true /false
*
* Determine if a C++ member function or member function template is
* pure virtual.
*/
VALUE
c_Cursor_cxx_method_is_pure_virtual(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_CXXMethod_isPureVirtual(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#cxx_method_is_static => true /false
*
* Determine if a C++ member function or member function template is
* static.
*/
VALUE
c_Cursor_cxx_method_is_static(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_CXXMethod_isStatic(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}
/**
* call-seq:
*   Clangc::Cursor#cxx_method_is_virtual => true /false
*
* Determine if a C++ member function or member function template is
* explicitly declared 'virtual' or if it overrides a virtual method from
* one of the base classes.
*/
VALUE
c_Cursor_cxx_method_is_virtual(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_CXXMethod_isVirtual(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#cxx_method_is_const => true /false
*
* Determine if a C++ member function or member function template is
* constant.
*/
VALUE
c_Cursor_cxx_method_is_const(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  if(clang_CXXMethod_isConst(c->data) != 0)
    return Qtrue;
  else
    return Qfalse;
}

/**
* call-seq:
*   Clangc::Cursor#template_cursor_kind => Clangc::CursorKind.constants
*
* Given a cursor that represents a template, determine the cursor kind of
* the specializations would be generated by instantiating the template.
*
* This routine can be used to determine what flavor of function template,
* class template, or class template partial specialization is stored in the
* cursor. For example, it can describe whether a class template cursor is
* declared with "struct", "class" or "union".
*
* C The cursor to query should represent a template declaration.
*
* It returns the cursor kind of the specializations that would be generated
* by instantiating the template . If the cursor is not a template, it returns
* Clangc::CursorKind::NO_DECL_FOUND.
*/
VALUE
c_Cursor_get_template_cursor_kind(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  return CUINT_2_NUM(clang_getTemplateCursorKind(c->data)); 
}

/**
* call-seq:
*   Clangc::Cursor#specialized_cursor_template => Clangc::Cursor
*
* Given a cursor that may represent a specialization or instantiation
* of a template, retrieve the cursor that represents the template that it
* specializes or from which it was instantiated.
*
* This routine determines the template involved both for explicit 
* specializations of templates and for implicit instantiations of the template,
* both of which are referred to as "specializations". For a class template
* specialization (e.g., \c std::vector<bool>), this routine will return 
* either the primary template (\c std::vector) or, if the specialization was
* instantiated from a class template partial specialization, the class template
* partial specialization. For a class template partial specialization and a
* function template specialization (including instantiations), this
* this routine will return the specialized template.
*
* For members of a class template (e.g., member functions, member classes, or
* static data members), returns the specialized or instantiated member. 
* Although not strictly "templates" in the C++ language, members of class
* templates have the same notions of specializations and instantiations that
* templates do, so this routine treats them similarly.
*
* The cursor  may be a specialization of a template or a member
* of a template.
*
* If the given cursor is a specialization or instantiation of a 
* template or a member thereof, it returns the template or member that it specializes or
* from which it was instantiated. Otherwise, returns a NULL cursor.
*/
VALUE
c_Cursor_get_specialized_cursor_template(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  Cursor_t *sct;
  VALUE specialized_cursor_template;
  R_GET_CLASS_DATA("Clangc", "Cursor", specialized_cursor_template, Cursor_t, sct);
  sct->data = clang_getSpecializedCursorTemplate(c->data);
  sct->parent = c->parent;
  return specialized_cursor_template;
}

/**
* call-seq:
*   Clangc::Cursor#completion_string => Clangc::CompletionString
*
* Retrieve a completion string for an arbitrary declaration or macro
* definition cursor.
*
* It returns a non-context-sensitive completion string for declaration and macro
* definition cursors, or nil for other kinds of cursors.
*/
VALUE
c_Cursor_get_completion_string(VALUE self)
{
  Cursor_t *c;
  Data_Get_Struct(self, Cursor_t, c);
  CompletionString_t *cs;
  VALUE completion_string;
  R_GET_CLASS_DATA("Clangc", "CompletionString", completion_string, CompletionString_t, cs);
  cs->data = clang_getCursorCompletionString(c->data);
  cs->parent = self;
  if(cs->data == NULL)
    return Qnil;
  else
    return completion_string;
}

/**
* clang_Cursor_getTranslationUnit won't be implemented
* The memory managment would be too tricky
*/
