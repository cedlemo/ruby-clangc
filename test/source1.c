#include <stdio.h>

typedef struct data {
  int index;
  char * array[256];
  } data_t;

int
stupid_function (double d) {
  printf("This is a stupid function\n");
  struct data o;
  return 0;
}

int
main (int argc, char **argv) {
  int x __attribute__ ((aligned (16))) = 0;
  return 0;
}

