#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "linked_list.h"

// typedef struct listnode listnode;

/* Structure for node of the linked list */
typedef struct listnode {
  void *key;
  void *value;
  struct listnode *next;
} listnode;

typedef struct listnode listnode;

/* Linked list structure which tracks the head and tail
 * Comparator is a pointer to a function which takes in 
 * 2 objects of ADT similar to the equals function in JAVA
 */ 
struct linked_list {
  listnode *head;
  comparator cmp;
}; 

/* Free resources allocated to each node */
void free_listnode(listnode *node) {
  free(node->key);
  free(node->value);
  free(node);
}

/* Free resources allocated to linked list */
void free_list(linked_list *list) {
  listnode *prev = list->head, *curr = prev->next;
  
  for (; curr; prev = curr, curr = curr->next) {
    free_listnode(prev);
  }  
  free_listnode(prev);
  free(list);
}

/* Create a node for the linked list 
 * 
 *  @param:
 *    - key: key of the node
 *    - key_size: size of of key
 *    - value: value held by the node
 *    - next: pointer to the next node 
 *  
 *  Note:
 *    - the node makes a copy of the key and stores
 *      the pointer to the value
 *  @return: pointer to new node, NULL if anything fails
 */
listnode *create_listnode(void *key, size_t key_size, void *value, listnode *next) {
  listnode *node = calloc(1, sizeof(listnode));
  if (!node) return NULL;
 
  node->next = next;
  node->value = value;
  node->key = malloc(key_size);
  memcpy(node->key, key, key_size);
  
  if (!node->key) {
    free(node);
    return NULL;
  }
  return node;
}

linked_list *create_linked_list(comparator cmp) {
  linked_list *list = (linked_list *) calloc(1, sizeof(linked_list));
  list->head = create_listnode(NULL, 0, NULL, NULL);
  list->cmp = cmp;

  if (!list || !list->head) free_list(list);
  
  return list;
}

bool hash_contains(linked_list *list, void *key) {
  if (!key) return false;
  bool found = false;

  for (listnode *curr = list->head; !found && curr; curr = curr->next){
    found = (list->cmp)(curr->key, key) == 0;
  }

  return found;
}

bool list_insert(linked_list *list, void *key, size_t key_size, void *value) {
  if (!key)  return false;
  listnode *curr;

  for (curr = list->head; curr->next; curr = curr->next) {
    /* Duplicate key found */
    if ((list->cmp)(curr->next->key, key) == 0)
      return false;
  }
  
  listnode *node = create_listnode(key, key_size, value, NULL);
  curr->next = node;
  return true;
}

bool list_delete(linked_list *list, void *key) {
  if (!key)  return false;
  listnode *prev = list->head, *curr = list->head;
  
  for (; curr; prev = curr, curr = curr->next) {
    /* Found key */
    if ((list->cmp)(curr->key, key) == 0){
      prev->next = curr->next;
      free_listnode(curr);
      return true;
    }  
  }
  return false;
}

void *list_find(linked_list *list, void *key) {
  if (!key)  return false;
  listnode *curr;
  
  for (curr = list->head; curr; curr = curr->next) {
    /* Found key */
    if ((list->cmp)(curr->key, key) == 0){
      return curr->value;
    }  
  }
  return NULL;
}

