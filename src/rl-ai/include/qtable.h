#ifndef Q_TABLE_H
#define Q_TABLE_H

#include "hash_table.h"
#include "env_defns.h"


int compare_qdata(void *this, void *that);

int hash_qdata(void *ptr);

q_table *init_qtable();

bool insert_qtable(q_table *table, env_t *qdata, void *value);

bool load_hash_qtable(q_table *table, long *hash, void *value);

void *find_qtable(q_table *table, env_t *qdata);


#endif
