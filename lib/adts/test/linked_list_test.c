#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "testutils.h"


#define TEST_INSERT(sign)\
  testbool(sign list_insert(list, key, key_size, (void *) value), test_name)
  
#define TEST_FIND(sign)\
  testbool(sign list_find(list, key), test_name)

#define TEST_REMOVE(sign)\
  testbool(sign list_delete(list, key), test_name)

 
int mod_strcmp(void *this, void *that){
  return strcmp((const char *) this, (const char *) that);
}

/* The linkedlist should be ADTs but here I am only testing for
 * key as a string at the moment
 */
int main() {
  size_t key_size;
  char *key = malloc(sizeof(char) * 50);
  char *test_name;

  int *value = malloc(sizeof(int));

  linked_list *list = create_linked_list(&mod_strcmp);
  
  /* Initialise list correctly */
  test_name = "List initialised correctly";
  testbool(list, test_name);

  /* Find an element in empty list*/
  test_name = "Find element in empty list should return 'False'";
  key_size = strlen(test_name);
  TEST_FIND(!);

  /* New inserts */
  test_name = "Inserting 'First' should return 'True'";
  key = "First";
  *value = 1;
  key_size = strlen(key);
  TEST_INSERT();
  
  /* Test get */
  test_name = "Get should return correct value";
  int *res = (int *) list_find(list, key);
  testint(*res, 1, test_name);

  /* Repeated inserts */
  test_name = "Inserting 'First' again must return 'False'";
  TEST_INSERT(!);
 
  /* Remove existing element */
  test_name = "Removing 'First' should return 'True'";
  TEST_REMOVE();
  
  /* 'First' should no longer exists */
  test_name = "'First' should not be found anymore";
  TEST_FIND(!);

  /* Second removal of 'First' */
  test_name = "Trying to remove 'First' again should return 'False'";
  TEST_REMOVE(!);

  /* Deallocate memory */
  free_list(list);
}
