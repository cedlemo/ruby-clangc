/**
 *  A reference to a set of overloaded functions or function templates
 * that has not yet been resolved to a specific function or function template.
 *
 * An overloaded declaration reference cursor occurs in C++ templates where
 * a dependent name refers to a function. For example:
 *
 **/

template<typename T> void swap(T&, T&);

struct X { ... };
void swap(X&, X&);

template<typename T>
void reverse(T* first, T* last) {
  while (first < last - 1) {
    swap(*first, *--last);
    ++first;
  }
}

struct Y { };
void swap(Y&, Y&);

/**
 * Here, the identifier "swap" is associated with an overloaded declaration
 * reference. In the template definition, "swap" refers to either of the two
 * "swap" functions declared above, so both results will be available. At
 * instantiation time, "swap" may also refer to other functions found via
 * argument-dependent lookup (e.g., the "swap" function at the end of the
 * example).
 *
 * The functions \c clang_getNumOverloadedDecls() and 
 * \c clang_getOverloadedDecl() can be used to retrieve the definitions
 * referenced by this cursor.
 **/

