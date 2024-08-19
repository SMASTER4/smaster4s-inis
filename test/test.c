#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include "io.h"

#define access _access_s
#define F_OK 0
#define W_OK 2
#define R_OK 4
#endif

#include <smaster4s-inis.h>

#include "test.h"

static void _assert_string(const char* name, const char* a, const char* b) {
  if(name == NULL) {
    printf("Assertion failed because as name of the assertion NULL was passed\n");
    return;
  }
  if(a == NULL || b == NULL) {
    printf("Assertion %s failed, because one of the output values is NULL\n", name);
    return;
  }
  if(strcmp(a, b) != 0) {
    printf("Assertion %s failed with the values: %s, %s\n", name, a, b);
    return;
  }
}

static void _test_ini_get_char() {
  if(access("test.ini", F_OK) == 0 && access("test.ini", R_OK) != 0)
    return;
  if(access("test.ini", F_OK) != 0) {
    FILE* file = fopen("test.ini", "w");
    if(file == NULL)
      return;
    fprintf(file, "[section]\nkey=value# Comment\n");
    fclose(file);
  }
  char* result = ini_get_char("test.ini", "section", "key");
  _assert_string("ini_get_char", result, "value");
  if(result != NULL)
    free(result);
}

static void _test_ini_get_char_clean_up() {
  if(access("test.ini", W_OK))
    remove("test.ini");
}

static void _run_tests(int count) {
  double start = (double) clock() / CLOCKS_PER_SEC;
  for(int i = 0; i < count; i++) {
    _test_ini_get_char();
    _test_ini_get_char_clean_up();
  }
  double end = (double) clock() / CLOCKS_PER_SEC;
  printf("Finished running assertion loop %d times in %f on average in %f seconds!\n", count, (end - start) / count, end - start);
}

int main(int argc, char** argv) {
  if(argc >= 2)
    _run_tests(atoi(argv[1]));
  else
    _run_tests(1);
}
