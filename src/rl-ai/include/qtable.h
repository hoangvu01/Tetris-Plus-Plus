#ifndef Q_TABLE_H
#define Q_TABLE_H

#include "hash_table.h"
#include "env_defns.h"

/* Compare data of 2 queue table */
int compare_qdata(void *this, void *that);

/* Hash function for a q_data */
int hash_qdata(void *ptr);

/* Allocated resources for qtable */
q_table *init_qtable();

/* Add element into qtable */
bool insert_qtable(q_table *table, env_t *qdata, void *value);

/* Find an element (type env_t) in qtable */
void *find_qtable(q_table *table, env_t *qdata);

/* Free resources allocated to q_table */
void free_qtable(q_table *table);

#endif

