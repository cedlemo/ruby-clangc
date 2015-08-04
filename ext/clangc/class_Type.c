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
/*Type ruby class*/
#include "class_Type.h"
#include "macros.h"

static void
c_Type_struct_free(Type_t *s)
{
  if(s)
  {
    
    ruby_xfree(s);
  }
}  

static void
c_Type_mark(void *s)
{
  if(s)
  {
    Type_t *t =(Type_t *)s;
    rb_gc_mark(t->parent);
  }
}
VALUE
c_Type_struct_alloc( VALUE klass)
{
  
  return Data_Wrap_Struct(klass, NULL, c_Type_struct_free, ruby_xmalloc(sizeof(Type_t)));
}
/**
* call-seq:
*   Clangc::Type#kind => Fixnum
*
* Get the kind of type. The returned value is a postive integer contained in
* Clangc::TypeKind.constants
*/
VALUE
c_Type_get_kind(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  return CUINT_2_NUM(t->data.kind);
}

/**
* call-seq:
*   Clangc::Type#spelling => String
*
* Pretty-print the underlying type using the rules of the
* language of the translation unit from which it came.
*
* If the type is invalid, an empty string is returned.
*/
VALUE
c_Type_get_spelling(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  CXString str = clang_getTypeSpelling(t->data);
  VALUE spelling = rb_str_new2( clang_getCString(str) );
  clang_disposeString(str);
  return spelling;
}

/**
* call-seq:
*   Clangc::Type#is_equal(Clangc::Type) => boolean
*
* Determine whether two Clangc::Type represent the same type.
*
* Returns true if the Clangc::Type represent the same type and
* false otherwise.
*/
VALUE
c_Type_is_equal(VALUE self, VALUE type)
{
  Type_t *t1;
  Type_t *t2;
  Data_Get_Struct(self, Type_t, t1);
  Data_Get_Struct(type, Type_t, t2);
  return clang_equalTypes(t1->data, t2->data) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Type#canonical_type => Clangc::Type
*
* Return the canonical type for a Clangc::Type.
*
* Clang's type system explicitly models typedefs and all the ways
* a specific type can be represented.  The canonical type is the underlying
* type with all the "sugar" removed.  For example, if 'T' is a typedef
* for 'int', the canonical type for 'T' would be 'int'.
*/
VALUE
c_Type_get_canonical_type(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  Type_t *c;
  VALUE canonical;
  R_GET_CLASS_DATA("Clangc", "Type", canonical, Type_t, c);
  c->data = clang_getCanonicalType(t->data);
  c->parent = t->parent;
  return canonical;
}

/**
* call-seq:
*   Clangc::Type#pointee_type => Clangc::Type
*
* For pointer types, returns the type of the pointee.
*/
VALUE
c_Type_get_pointee_type(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  Type_t *p;
  VALUE pointee;
  R_GET_CLASS_DATA("Clangc", "Type", pointee, Type_t, p);
  p->data = clang_getPointeeType(t->data);
  p->parent = t->parent;
  return pointee;
}

/**
* call-seq:
*   Clangc::Type#is_const_qualified => true/false
*
* Determine whether a Clangc::Type instance has the "const" qualifier set,
* without looking through typedefs that may have added "const" at a
* different level.
*/
VALUE
c_Type_is_const_qualified(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  return clang_isConstQualifiedType(t->data) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Type#is_volatile_qualified => true/false
*
* Determine whether a Clangc::Type instance has the "volatile" qualifier set,
* without looking through typedefs that may have added "const" at a
* different level.
*/
VALUE
c_Type_is_volatile_qualified(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  return clang_isVolatileQualifiedType(t->data) == 0 ? Qfalse : Qtrue;
}

/**
* call-seq:
*   Clangc::Type#is_restrict_qualified => true/false
*
* Determine whether a Clangc::Type instance has the "restrict" qualifier set,
* without looking through typedefs that may have added "const" at a
* different level.
*/
VALUE
c_Type_is_restrict_qualified(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  return clang_isRestrictQualifiedType(t->data) == 0 ? Qfalse : Qtrue;
}

/**
* Retrieve the return type associated with a function type.
*
* If a non-function type is passed in (Clangc::Type#kind != Clangc::TypeKind::FunctionNoProto for example),
* an invalid type is returned.
*/
VALUE
c_Type_get_result_type(VALUE self)
{
  Type_t *t;
  Data_Get_Struct(self, Type_t, t);
  Type_t *r;
  VALUE result;
  R_GET_CLASS_DATA("Clangc", "Type", result, Type_t, r);
  r->data = clang_getResultType(t->data);
  r->parent = t->parent;
  return result;
}
