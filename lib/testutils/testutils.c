/*
 * This testutils module is originally written by Duncan White.
 * It has been adapted to provide extra functionalities.
 * More infomation on this module can be found at:
 * https://www.doc.ic.ac.uk/~dcw/c-tools-2020/lecture2/
 */

#include "testutils.h"

void switch_color(bool condition) {
  if (condition) {
    /* Switch to green */
    printf("\033[0;32m");
  } else {
    /* Switch to red */
    printf("\033[0;31m");
  }
}

void reset_color() { printf("\033[0m"); }

void testbool(bool condition, char *testname) {
  switch_color(condition);
  printf("T %s: %s\n", testname, condition ? "OK" : "FAIL");
  reset_color();
}

void testint(int got, int expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=%d, got=%d): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void testlong(long got, long expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=%ld, got=%ld): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void testdouble(double got, double expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=%g, got=%g): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void teststring(char *got, char *expected, char *testname) {
  switch_color(strcmp(expected, got) == 0);
  printf("T %s (expected='%s', got='%s'): %s\n", testname, expected, got,
         strcmp(expected, got) == 0 ? "OK" : "FAIL");
  reset_color();
}

void testword(uint32_t got, uint32_t expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=0x%08x, got=0x%08x): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void testbyte(uint8_t got, uint8_t expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=0x%08x, got=0x%08x): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void testsize(size_t got, size_t expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=0x%zu, got=0x%zu): %s\n", testname, expected, got,
         expected == got ? "OK" : "FAIL");
  reset_color();
}

void testpointer(void *got, void *expected, char *testname) {
  switch_color(got == expected);
  printf("T %s (expected=%p, got=%p): %s\n", testname, expected,
         got,expected == got ? "OK" : "FAIL");
  reset_color();
}
