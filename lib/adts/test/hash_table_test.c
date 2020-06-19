#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "testutils.h"
#include "hash_table.h"

#define new_int() malloc(sizeof(int));

#define TEST_HASH_CONTAINS(sign)\
  testbool(sign hash_contains(table, (void *)key), testname)

#define TEST_HASH_INSERT(sign, value)\
  testbool(sign hash_insert(table, (void *)key, strlen(key) + 1, (void *)value), testname)

#define TEST_HASH_DELETE(sign)\
  testbool(sign hash_delete(table, (void *)key), testname)


int str_cmp(void *this, void *that) {
  return strcmp((const char *) this, (const char *) that);
}

int hash_str(void *this) {
  char *str = (char *) this;
  int hash = 0;
  for (int i = 0; i < strlen(str); i++) {
    hash += str[i];
  }
  return hash;
}

int main() {
  char *testname, *key;
  int *res;
  int initial_size = 5;

  /* Hash table initialised correctly */
  testname = "Hash Table initialised correctly";
  hash_table *table = create_hashtable(&str_cmp, &hash_str, initial_size);
  testbool(table, testname);

  /* Query an empty table */
  testname = "Querying empty table should return 'False'";
  key = "abc";
  TEST_HASH_CONTAINS(!);

  /* Insert first item should return correctly */
  testname = "Inserting 'First' element should return 'True'";
  key = "First";
  int *value_first = new_int(); 
  *value_first = 0;
  TEST_HASH_INSERT(, value_first);

  /* Insert duplicate item */
  testname = "Inserting 'First' should return 'False' the second time";
  TEST_HASH_INSERT(!, value_first);
  

  /* Inserting second item */
  testname = "Inserting 'Second' should return 'True'";
  key = "Second";
  int *value_second = new_int();
  *value_second = 1;
  TEST_HASH_INSERT(, value_second);

  /* Inserting second item again */
  testname = "Inserting 'Second' again should return 'False'";
  TEST_HASH_INSERT(!, value_second);


  /* Table should contains 'Second' */
  testname = "Table should contain key 'Second' now";
  key = "Second";
  TEST_HASH_CONTAINS();

  /* Table should contains 'First' */
  testname = "Table should now contain 'First' key";
  key = "First";
  TEST_HASH_CONTAINS();

  /* Key should be case sensitive */
  testname = "Key should be case sensitive thus not contain 'fIrSt'";
  key = "fIrSt";
  TEST_HASH_CONTAINS(!);  
  
  /* Items with the same hash should still be inserted */
  testname = "Hash collision resolved correctly 'tFsir' (anagram of 'First')";
  key = "fIrSt";
  int *value_first_scrambled = new_int();
  *value_first_scrambled = 0;
  TEST_HASH_INSERT(, value_first_scrambled);

  /* Items with the same hash should still be inserted */
  testname = "Hash collision resolved correctly 'neScod' (anagram of 'Second')";
  key = "neScod";
  int *value_second_scrambled = new_int();
  *value_second_scrambled = 1;
  TEST_HASH_INSERT(, value_second_scrambled);

  
  /* Testing get on 'First' */
  testname = "Get return pointer to int correctly for 'First'";
  res = (int *) hash_find(table, "First");
  testint(*res, 0, testname);

  /* Testing get on 'Second' */
  testname = "Get return pointer to int correctly for 'Second'";
  res = (int *) hash_find(table, "Second");
  testint(*res, 1, testname);
  
  /* Testing get on non-existing item */
  testname = "Get return pointer to int correctly for non-existing item";
  res = (int *) hash_find(table, "non-exists");
  testbool(res == NULL, testname);
  
  /* Removing 'First' */
  testname = "Removing 'First' should return 'True'";
  key = "First";
  TEST_HASH_DELETE();

  /* Removing 'First' again */
  testname = "Removing 'First' again should return 'False'";
  TEST_HASH_DELETE(!);

  /* Removing 'Second' */
  testname = "Removing 'Second' should return 'True'";
  key = "Second";
  TEST_HASH_DELETE();

  /* Removing 'Second' again */
  testname = "Removing 'Second' again should return 'False'";
  TEST_HASH_DELETE(!);

  printf("----------------------------------------------------\n");
  printf("Hash table initial size: %d, trying to overflow.....\n", initial_size);
  printf("----------------------------------------------------\n");

  /* Insert third item should return correctly */
  testname = "Inserting 'Third' element should return 'True'";
  key = "Third";
  int *value_third = new_int();
  *value_third = 2;
  TEST_HASH_INSERT(, value_third);
  
  testname = "Get returns pointer correctly for 'Third'";
  res = hash_find(table, "Third");
  testint(*res, 2, testname);
  
  /* Insert fourth item should return correctly */
  testname = "Inserting 'Fourth' element should return 'True'";
  key = "Fourth";
  int *value_fourth = new_int();
  *value_fourth = 3;
  TEST_HASH_INSERT(, value_fourth);

  testname = "Get returns pointer correctly for 'Fourth'";
  res = hash_find(table, "Fourth");
  testint(*res, 3, testname);
  
  /* Insert fifth item should return correctly */
  testname = "Inserting 'Fifth' element should return 'True'";
  key = "Fifth";
  int *value_fifth = new_int();
  *value_fifth = 4;
  TEST_HASH_INSERT(, value_fifth);
  
  testname = "Get returns pointer correctly for 'Fifth'";
  res = hash_find(table, "Fifth");
  testint(*res, 4, testname);
  
  /* Contains 3 items recently added */
  testname = "Contains 'Third' item";
  key = "Third";
  TEST_HASH_CONTAINS();
  

  testname = "Contains 'Fourth' item";
  key = "Fourth";
  TEST_HASH_CONTAINS();

  testname = "Contains 'Fifth' item";
  key = "Fifth";
  TEST_HASH_CONTAINS();

  
  printf("----------------------------------------------------\n");
  printf("Hash table should've been resized once, now 3x......\n");
  printf("----------------------------------------------------\n");

  char *test = malloc(sizeof(char) * 100);
  char *test_key = malloc(sizeof(char) * 50);
  for (int i = 0; i < 50; i++) {
    char *val = new_int();
    *val = i;
    sprintf(test, "Iteration #%-2d added correctly", i);
    sprintf(test_key, "Itr #%-2d", i);
    
    testbool(hash_insert(table, (void *)test_key, strlen(test_key), (void *) val), test);
  }

  for (int i = 0; i < 50; i++) {
    sprintf(test, "Iteration #%-2d pointer to value returns correctly", i);
    sprintf(test_key, "Itr #%-2d", i);
    res = hash_find(table, (void *) test_key);
    testint(*res, i, test);
  }
  
  free(test);
  free(test_key);
  free_hashtable(table);
 
  printf("Hash Table freed without segfault YAYY\n");
  return 0;
}
