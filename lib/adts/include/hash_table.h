
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "common.h"

typedef struct hash_table hash_table;

typedef int (*hashfunc) (void *);

/*  
 *  Create a generic linked table 
 *  @param:
 *    - cmp: comparator for 2 objects of type of keys in the table
 *    - hashcode: pointer to function to compute hashcode from the key
 *    - initial_size: initial_size of array
 *  @return: 
 *    - pointer to linked table on success
 *    - NULL if anything fails
 */
hash_table *create_hashtable(comparator cmp, hashfunc hashcode, size_t initial_size);

/* 
 *  Find an element in a hash table 
 *  @param: 
 *    - table: pointer to hash_table 
 *    - key: pointer to key to find 
 *  @return: true iff key is found
 */
bool hash_contains(hash_table *table, void *key);

/* 
 *  Add item in to hash table
 *  @param: as above
 *      - key_size: size of the key in BYTES 
 *  @return: true iff the table has been modified
 */
bool hash_insert(hash_table *table, void *key, size_t key_size, void *value);

/* Remove an key from hash table */
bool hash_delete(hash_table *table, void *key); 

/* Free memory allocated to hash_table */
void free_hashtable(hash_table *table);

/* Returns key if present, NULL otherwise */
void *hash_find(hash_table *table, void *key);


#endif
