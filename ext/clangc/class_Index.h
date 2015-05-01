#ifndef INDEX_H
#define INDEX_H
  #include <ruby/ruby.h>
typedef struct Index_t {
    CXIndex data; 
} Index_t;

  VALUE generate_Index_under(VALUE, VALUE);
#endif //INDEX_H
