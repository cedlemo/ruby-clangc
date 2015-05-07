#ifndef TRANSLATIONUNIT_H
#define TRANSLATIONUNIT_H
#include <ruby/ruby.h>
typedef struct TranslationUnit_t {
    CXTranslationUnit data; 
} TranslationUnit_t;

VALUE
generate_TranslationUnit_under(VALUE, VALUE);
#endif //TRANSLATIONUNIT_H
