#include "qtable.h"

#define Q_SIZE 100
/*
int compare_env_state(void *this, void *that) {
  grid_t this_grid = ((env_t *) this)->grid;
  grid_t that_grid = ((env_t *) that)->grid;
 
  for (int i = 0; i < GHEIGHT; i++){
    for (int j = 0; j < GWIDTH; j++) {
      if ((this_grid[i][j] > 0) ^ (that_grid[i][j] > 0))
        return this_grid[i][j] - that_grid[i][j];
    }
  }

  return ((env_t *) this)->block - ((env_t *) that)->block;

}
*/
long hash_env_state(env_t *env) {
  long hash = 0;
  for (int i = 0; i < GHEIGHT; i++){
    for (int j = 0; j < GWIDTH; j++) {
      hash += (env->grid[i][j] > 0) * (i * GWIDTH + j);
    }
  }
  
  hash += (env->block - env->list) * 5381;
  hash %= 20000;
  
  return hash;


}

long compare_hash(void *this, void *that) {
  return *((long *) this) - *((long *) that);
}

long identity(void *ptr) {
  return *((long *) ptr);
}

q_table *init_qtable() {
  return create_hashtable(&compare_hash, &identity, Q_SIZE);  
}

bool insert_qtable(q_table *table, env_t *env_state, void *value) {
  long env_hash = hash_env_state(env_state);
  return hash_insert(table, (void *) &env_hash, sizeof(long), value);
}

bool load_hash_qtable(q_table *table, long *hash, void *value) {
  return hash_insert(table, hash, sizeof(long), value);
}

void *find_qtable(q_table *table, env_t *env_state) {
  long env_hash = hash_env_state(env_state);
  return hash_find(table, (void *) &env_hash);
}
