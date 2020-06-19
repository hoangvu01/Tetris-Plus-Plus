#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "symbol_table.h"
#include "testutils.h"


#define TEST_INSERT(sign, value)\
  testbool(sign insert_literal(table, label, value), test_name)

#define TEST_GET(expected)\
  testword(*((int *) get_literal(table, label)), expected, test_name)

int main() {
  
  char *test_name;
  char *label;
  symbol_table_t *table;
  
  /* Table should be created */
  test_name = "Table should be created successfully";
  table = create_table();
  testbool(table, test_name);


  /* Symbol should be added correctly */
  test_name = "First symbol should be added correctly";
  label = "Foo";
  int *value1 = malloc(sizeof(int));
  *value1 = 0;
  TEST_INSERT(, value1);

  /* Adding the same symbol again */
  test_name = "Adding the same symbol should not modify the table";
  TEST_INSERT(!, NULL);

  
  /* New symbol should be added correctly */
  test_name = "Second symbol with the same literal should still be added";
  label = "Bar";
  int *value2 = malloc(sizeof(int));
  *value2 = 0;
  TEST_INSERT(, value2);
  
  /* New symbol with same label should be discarded */
  test_name = "Symbol with the same label as the first should not be added";
  label = "Foo";
  TEST_INSERT(!, value2);
  
  /* New symbol should be added correctly */
  test_name = "Third symbol to test for 'get' added correctly";
  label = "Label";
  int *value3 = malloc(sizeof(int));
  *value3 = 123456;
  TEST_INSERT(, value3);
  
  /* Get literal using label */
  test_name = "Get literal of a label works correctly";
  label = "Label";
  TEST_GET(123456);

  free_table(table);
  return 0;
}
