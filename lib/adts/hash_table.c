#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "hash_table.h"

#define assert_not_null(item)\
  if (!item) return NULL 

#define assert_array(func)\
  do {if(!table->arr) {\
    fprintf(stderr, func); return false;\
  }} while(0)

#define LOAD_FACTOR 0.75

typedef struct hashnode {
  void *key;
  void *value;
  size_t key_size;
} hashnode;

typedef struct hash_table {
  size_t size, max_size;
  hashnode **arr;
  comparator cmp;
  hashfunc hashcode;
} hash_table ;

/* Helper function for using hash to index into array */
static int get_hash_index(hash_table *table, void *key) {
  int hash = table->hashcode(key);
  return hash % (table->max_size);
}

/* Free resources allocated to hashnode */
static void free_hashnode(hashnode *node) {
  free(node->value);
  free(node->key);
  free(node);
}

/* Free resources allocated to hashtable */
void free_hashtable(hash_table *table) { 
  if (table->arr != NULL) {
    for (int i = 0; i < table->max_size; i++) 
      if (table->arr[i] != NULL) free_hashnode(table->arr[i]);
  }
  free(table->arr);
  free(table);
}

/* Allocate memory for a hashnode */
static hashnode *create_hashnode(void *key, size_t key_size, void *value){
  hashnode *node = malloc(sizeof(hashnode));
  assert_not_null(node);
  
  node->key_size = key_size;
  node->value = value;
  node->key = malloc(key_size);
  memcpy(node->key, key, key_size);

  if (!node->key) {
    free(node);
    return NULL;
  }
  return node;
}

/* Allocate memory for a hashtable */
hash_table *create_hashtable(comparator cmp, hashfunc hashcode, size_t initial_size) {
  if (initial_size == 0) return false;
  hash_table *table = (hash_table *) calloc(1, sizeof(hash_table));
  assert_not_null(table);

  table->size = 0;
  table->cmp = cmp;
  table->hashcode = hashcode;
  table->max_size = initial_size;
  table->arr = (hashnode **) calloc(initial_size, sizeof(hashnode *));

  if (table->arr == NULL) free_hashtable(table);
  return table;
}

/*  Helper function for rehashing to transfer data between 
 *  2 hash tables
 */
static void transfer_data(hash_table *from, hash_table *to) {
  free(to->arr);
  to->hashcode = from->hashcode;
  to->max_size = from->max_size;
  to->size = from->size;
  to->cmp = from->cmp;
  to->arr = from->arr;
  
  free(from);
}

/* Rehashing tables if size ratio greater than LOAD FACTOR */
static bool rehashing(hash_table *table) {
  hash_table *new_table 
    = create_hashtable(table->cmp, table->hashcode, table->max_size * 2);

  for (int i = 0; i < table->max_size; i++) {
    if (!table->arr[i]) continue;
    
    hashnode *node = table->arr[i];
    hash_insert(new_table, node->key, node->key_size, node->value);
    free(node->key);
    free(node);
  }
  
  transfer_data(new_table, table);
  return true;
}

bool hash_contains(hash_table *table, void *key) {
  int index = get_hash_index(table, key);
  assert_array("_hash_contains_");
  
  while(table->arr[index] != NULL) {
    if (table->cmp(table->arr[index]->key, key) == 0)
      return true;
    index = (index + 1) % table->max_size;
  }
  return false;
}

/* Helper function for insert to check if rehashing is required */
static bool required_rehashing(hash_table *table) {
  double size = table->size + 1;
  double max_size = table->max_size;
  return (size / max_size) > LOAD_FACTOR;
}

bool hash_insert(hash_table *table, void *key, size_t key_size, void *value) {
  int index = get_hash_index(table, key);
  assert_array("_hash_insert_");
 
  while (table->arr[index] != NULL) {
    if (table->cmp(table->arr[index]->key, key) == 0)
      return false;
    index = (index + 1) % table->max_size;
  }
  
  /* Hash table has enough capacity */
  if (!required_rehashing(table)) {
    table->size += 1;
    table->arr[index] = create_hashnode(key, key_size, value);
    return true;
  }
  /* Recomputes insert after table is rehashed */
  if (rehashing(table)) return hash_insert(table, key, key_size, value);
  /* Rehasing failed */
  return false;
}

bool hash_delete(hash_table *table, void *key) {
  int index = get_hash_index(table, key);
  assert_array("_hash_delete_");

  while (table->arr[index]!= NULL) {
    if (table->cmp(table->arr[index]->key, key) == 0) {
      free_hashnode(table->arr[index]);
      table->arr[index] = NULL;
      table->size--;
      return true;
    }  
    index = (index + 1) % table->max_size;
  }
  return false;
}


void *hash_find(hash_table *table, void *key) {
  int index = get_hash_index(table, key);
  assert_array("_hash_find_");

  while(table->arr[index] != NULL) {
    if (table->arr[index]->key == NULL) return NULL;
    if (table->cmp(table->arr[index]->key, key) == 0) 
      return table->arr[index]->value;
    index = (index + 1) % table->max_size;
  }
  return NULL;
}

