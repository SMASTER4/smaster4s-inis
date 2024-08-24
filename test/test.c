#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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

static inline void _print_success(const char* name, const int count, const double start, const double end) {
  printf("Finished running the assertion loop of %s %d time%s in %f on average in %f seconds!\n", name, count, count > 1 ? "s" : "", (end - start) / count, end - start);
}

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

static void _test_ini_get_str_from_str(int count) {
  if(count <= 0)
    return;

  double start = (double) clock() / CLOCKS_PER_SEC;

  char result[256];
  for(int i = 0; i < count; i++) {
    ini_get_str_from_str(result, TEST_TEXT, "section", "key");
    _assert_string("ini_get_str_from_str", result, "value");
  }

  double end = (double) clock() / CLOCKS_PER_SEC;

  _print_success("ini_get_str_from_str", count, start, end);
}

static void _test_ini_get_str(int count) {
  if(count <= 0)
    return;
  if(_test_ini_get_str_prepare())
    printf("Preparing for the testing ini_get_char failed");

  double start = (double) clock() / CLOCKS_PER_SEC;

  char result[256];
  for(int i = 0; i < count; i++) {
    ini_get_str(result, "test.ini", "section", "key");
    _assert_string("ini_get_str", result, "value");
  }

  double end = (double) clock() / CLOCKS_PER_SEC;

  _test_ini_get_str_clean_up();

  _print_success("ini_get_str", count, start, end);
}

static bool _test_ini_get_str_prepare() {
  if(access("test.ini", F_OK) == 0 && access("test.ini", R_OK) != 0)
    return true;
  if(access("test.ini", F_OK) != 0) {
    FILE* file = fopen("test.ini", "w");
    if(file == NULL)
      return true;
    fprintf(file, TEST_TEXT);
    fclose(file);
  }
  return false;
}

static void _test_ini_get_str_clean_up() {
  if(access("test.ini", W_OK) == 0)
    remove("test.ini");
}

static void _run_tests(int count) {
  if(count <= 0) {
    printf("The execution count has to be higher than 0 not %d", count);
    return;
  }
  _test_ini_get_str(count);
  _test_ini_get_str_from_str(count);
}

int main(int argc, char** argv) {
  if(argc >= 2)
    _run_tests(atoi(argv[1]));
  else
    _run_tests(1);
}
