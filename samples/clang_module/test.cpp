// Try the following command:
// clang++ -fmodules -fcxx-modules -fmodules-cache-path=./cache_path -c test.cpp
// If you see stuff in the ./cache_path directory, then it works!
#include "test.h"

int main(int, char **) {
  return foo();
}
