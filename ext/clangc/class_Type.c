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
#include "class_Cursor.h"
#include "macros.h"

static void c_Type_struct_free(Type_t *s)
{
    if (s)
    {

        ruby_xfree(s);
    }
}

static void c_Type_mark(void *s)
{
    if (s)
    {
        Type_t *t = (Type_t *) s;
        rb_gc_mark(t->parent);
    }
}
VALUE
c_Type_struct_alloc(VALUE klass)
{

    return Data_Wrap_Struct(
        klass, NULL, c_Type_struct_free, ruby_xmalloc(sizeof(Type_t)));
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
    return CXSTR_2_RVAL(clang_getTypeSpelling(t->data));
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
    CHECK_ARG_TYPE(type, Type);
    Data_Get_Struct(type, Type_t, t2);
    return NOT_0_2_RVAL(clang_equalTypes(t1->data, t2->data));
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
    R_GET_CLASS_DATA("Clangc", Type, canonical, c);
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
    R_GET_CLASS_DATA("Clangc", Type, pointee, p);
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
    return NOT_0_2_RVAL(clang_isConstQualifiedType(t->data));
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
    return NOT_0_2_RVAL(clang_isVolatileQualifiedType(t->data));
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
    return NOT_0_2_RVAL(clang_isRestrictQualifiedType(t->data));
}

/**
* call-seq:
*   Clangc::Type#result_type => Clangc::Type
*
* Retrieve the return type associated with a function type.
*
* If a non-function type is passed in (Clangc::Type#kind !=
* Clangc::TypeKind::FunctionNoProto for example),
* an invalid type is returned.
*/
VALUE
c_Type_get_result_type(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    Type_t *r;
    VALUE result;
    R_GET_CLASS_DATA("Clangc", Type, result, r);
    r->data = clang_getResultType(t->data);
    r->parent = t->parent;
    return result;
}

/**
* call-seq:
*   Clangc::Type#calling_conv => Clangc::CallingConv
*
* Retrieve the calling convention associated with a function type.
*
* If a non-function type is passed in, Clangc::CallingConv::Invalid is returned.
*/
VALUE
c_Type_get_calling_conv(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CUINT_2_NUM(clang_getFunctionTypeCallingConv(t->data));
}
/**
* call-seq:
*   Clangc::Type#num_arg_types => Integer
*
* Retrieve the number of non-variadic parameters associated with a
* function type.
*
* If a non-function type is passed in, -1 is returned.
*/
VALUE
c_Type_get_num_arg_types(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CINT_2_NUM(clang_getNumArgTypes(t->data));
}

/**
* call-seq:
*   Clangc::Type#arg_type(Integer) => Clangc::Type
*
* Retrieve the type of a parameter of a function type.
*
* If a non-function type is passed in or the function does not have enough
* parameters, an invalid type is returned.
* Better alternative is
* <code>Clangc::Type#arg_types => Array</code>
*/
VALUE
c_Type_get_arg_type(VALUE self, VALUE index)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    int max = clang_getNumArgTypes(t->data);
    /*args number can be < 0 if self is not a function type
      In this case I set max to zero and let this method returns
      an invalid type. It the user responsabilitty to check
      if the Clangc::Type#kind is a Functionproto or Functionnoproto
    */
    if (max < 0) max = 0;
    unsigned int c_index = NUM2UINT(index);
    CHECK_IN_RANGE(c_index, 0, max);
    Type_t *a;
    VALUE arg;
    R_GET_CLASS_DATA("Clangc", Type, arg, a);
    a->data = clang_getArgType(t->data, c_index);
    a->parent = t->parent;
    return arg;
}

/**
* call-seq:
*   Clangc::Type#element_type => Clangc::Type
*
* Return the element type of an array, complex, or vector type.
*
* If a type is passed in that is not an array, complex, or vector type,
* an invalid type is returned.
*/
VALUE
c_Type_get_element_type(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    Type_t *e;
    VALUE element;
    R_GET_CLASS_DATA("Clangc", Type, element, e);
    e->data = clang_getElementType(t->data);
    e->parent = t->parent;
    return element;
}

/**
* call-seq:
*   Clangc::Type#num_elements => Num
*
* Return the number of elements of an array or vector type.
*
* If a type is passed in that is not an array or vector type,
* -1 is returned.
*/
VALUE
c_Type_get_num_elements(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CLLONG_2_NUM(clang_getNumElements(t->data));
}

/**
* call-seq:
*   Clangc::Type#array_element_type => Clangc::Type
*
* Return the element type of an array type.
*
* If a non-array type is passed in, an invalid type is returned.
*/
VALUE
c_Type_get_array_element_type(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    Type_t *e;
    VALUE element;
    R_GET_CLASS_DATA("Clangc", Type, element, e);
    e->data = clang_getArrayElementType(t->data);
    e->parent = t->parent;
    return element;
}

/**
* call-seq:
*   Clangc::Type#array_size => Num
*
* Return the array size of a constant array.
*
* If a non-array type is passed in, -1 is returned.
*/
VALUE
c_Type_get_array_size(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CLLONG_2_NUM(clang_getArraySize(t->data));
}

/**
* call-seq:
*   Clangc::Type#is_pod => true/false
*
*  Return true if the Clangc::Type is a POD (plain old data) type, and false
*  otherwise.
*/
VALUE
c_Type_is_pod(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return NOT_0_2_RVAL(clang_isPODType(t->data));
}

/**
* call-seq:
*  Clangc::Type#type_declaration => Clangc::Cursor
*
* Return the cursor for the declaration of the given type.
*/
VALUE
c_Type_get_type_declaration(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    Cursor_t *d;
    VALUE declaration;
    R_GET_CLASS_DATA("Clangc", Cursor, declaration, d);

    /* Here we get the parent of the parent of self
     * As self is a Clangc::Type, its parent is a Clangc::Cursor
     * and its parent is a TranslationUnit.
     * TL/DR The parent of the returned cursor is a TranslationUnit
     * */
    Cursor_t *c;
    Data_Get_Struct(t->parent, Cursor_t, c);

    d->data = clang_getTypeDeclaration(t->data);
    d->parent = c->parent;
    return declaration;
}

/**
* call-seq:
*   Clangc::Type#is_function_type_variadic => true/false
*
* Return true if the Clangc::Type is a variadic function type, and false
* otherwise.
*/
VALUE
c_Type_is_function_type_variadic(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return NOT_0_2_RVAL(clang_isFunctionTypeVariadic(t->data));
}

/**
 * call-seq:
 *  Clangc::Type#align_of => Number
 *
 * Return the alignment of a type in bytes as per C++[expr.alignof]
 *   standard.
 *
 * If the type declaration is invalid, Clangc::TypeLayoutError::INVALID is
 * returned.
 * If the type declaration is an incomplete type,
 * Clangc::TypeLayoutError::INCOMPLETE
 *   is returned.
 * If the type declaration is a dependent type,
 * Clangc::TypeLayoutError::DEPENDENT is
 *   returned.
 * If the type declaration is not a constant size type,
 *   Clangc::TypeLayoutError::NOT_CONSTANT_SIZE is returned.
 */
VALUE
c_Type_get_align_of(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CLLONG_2_NUM(clang_Type_getAlignOf(t->data));
}

/**
 * call-seq:
 *  Clangc::Type#size_of => Number
 *
 * Return the size of a type in bytes as per C++[expr.sizeof] standard.
 *
 * If the type declaration is invalid, Clangc::TypeLayoutError::INVALID is
 * returned.
 * If the type declaration is an incomplete type,
 * Clangc::TypeLayoutError::INCOMPLETE
 *   is returned.
 * If the type declaration is a dependent type,
 * Clangc::TypeLayoutError::DEPENDENT is
 *   returned.
 */
VALUE
c_Type_get_size_of(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CLLONG_2_NUM(clang_Type_getSizeOf(t->data));
}

/**
 * call-seq:
 *  Clangc::Type#class_type => Clangc::Type
 *
 * Return the class type of an member pointer type.
 *
 * If a non-member-pointer type is passed in, an invalid type is returned.
 */
VALUE
c_Type_get_class_type(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    VALUE class_type;
    Type_t *ct;
    R_GET_CLASS_DATA("Clangc", Type, class_type, ct);
    ct->data = clang_Type_getClassType(t->data);
    ct->parent = t->parent;
    return class_type;
}

/**
 * call-seq:
 *  Clangc::Type#offset_of(String) => Number
 *
 *  Return the offset of a field named S in a record of type T in bits
 *   as it would be returned by __offsetof__ as per C++11[18.2p4]
 *
 * If the cursor is not a record field, Clangc::TypeLayoutError::INVALID is
 * returned.
 * If the field's type declaration is an incomplete type,
 * Clangc::TypeLayoutError::INCOMPLETE
 *   is returned.
 * If the field's type declaration is a dependent type,
 * Clangc::TypeLayoutError::DEPENDENT is
 *   returned.
 * If the field's name S is not found,
 * Clanc::TypeLayoutError::INVALID_FIELD_NAME is returned
 */
VALUE
c_Type_get_offset_of(VALUE self, VALUE field)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CLLONG_2_NUM(clang_Type_getOffsetOf(t->data, RSTRING_2_CHAR(field)));
}

/**
 * call-seq:
 *  Clangc::Type#num_template_arguments => Integer
 *
 * Returns the number of template arguments for given class template
 * specialization, or -1 if type \c T is not a class template specialization.
 *
 * Variadic argument packs count as only one argument, and can not be inspected
 * further.
 */
VALUE
c_Type_get_num_template_arguments(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return CINT_2_NUM(clang_Type_getNumTemplateArguments(t->data));
}

/**
 * call-seq:
 *  Clangc::Type#template_argument_as_type => Clangc::Type
 *
 * Returns the type template argument of a template class specialization
 * at given index.
 *
 * This function only returns template type arguments and does not handle
 * template template arguments or variadic packs.
 */
VALUE
c_Type_get_template_argument_as_type(VALUE self, VALUE index)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    VALUE template_argument;
    Type_t *ta;
    R_GET_CLASS_DATA("Clangc", Type, template_argument, ta);
    ta->data = clang_Type_getTemplateArgumentAsType(t->data, NUM2UINT(index));
    ta->parent = t->parent;
    return template_argument;
}

/**
 * call-seq:
 *  Clangc::Type#cxx_ref_qualifier => Clangc::RefQualifiers
 *
 * Retrieve the ref-qualifier kind of a function or method.
 *
 * The ref-qualifier is returned for C++ functions or methods. For other types
 * or non-C++ declarations, CXRefQualifier_None is returned.
 */
VALUE
c_Type_get_cxx_ref_qualifier(VALUE self)
{
    Type_t *t;
    Data_Get_Struct(self, Type_t, t);
    return INT2NUM(clang_Type_getCXXRefQualifier(t->data));
}
