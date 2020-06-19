/*
 * symbol interface: inserting labels and its address, retrieving label address
 */

#ifndef TABLE_H
#define TABLE_H

#include "hash_table.h"

/* symbol_table_t is an implementation of the generic
 * interface hash_table 
 */
typedef hash_table symbol_table_t;

/* Create a table */
symbol_table_t *create_table();

/* Free resources allocated to table */
void free_table(symbol_table_t *table);

/* Insert a label and literal into table 
 *  @param:
 *    - table: pointer to the table to add to
 *    - label: string identifier of the row
 *    - value: pointer to value you want to insert to
 *  @return:
 *    - TRUE iff label has yet to be seen (i.e. table not modified)
 * 
 *  + value: is a pointer to a heap allocated position and when
 *  + the node is removed, this is going to be freed by the list
 */

bool insert_literal(symbol_table_t *table, char *label, void *value);

/* Returns a pointer to the value with associated label 
 *  NULL if label not found
 */
void *get_literal(symbol_table_t *table, char *label);
#endif
