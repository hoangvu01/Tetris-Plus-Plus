#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "symbol_table.h"

#define INITIAL_SIZE 20

static int hash(void *ptr) {
  char *str = (char *) ptr;
  int hashcode = 0;
  for (int i = 0; i < strlen(str); i++) {
    hashcode += (str[i] ^ hashcode) >> 1;
  }
  return hashcode % 5381;
}

static int symbol_comparator(void *this, void *that) {
  return strcmp((char *) this, (char *) that);
}

symbol_table_t *create_table() {
  return create_hashtable(&symbol_comparator, &hash, INITIAL_SIZE);
}

void free_table(symbol_table_t *table){
  free_hashtable(table);
}

bool insert_literal(symbol_table_t *table, char *label, void *literal) {
  return hash_insert(table, label, strlen(label) + 1, literal);
}

void *get_literal(symbol_table_t *table, char *label) {
  return hash_find(table, label); 
}

