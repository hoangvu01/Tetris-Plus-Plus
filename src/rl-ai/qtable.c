#include "qtable.h"

#include <stdlib.h>

#include "display.h"

#define Q_SIZE 100


long hash_env_state(void *env_ptr) {
  env_t *env = (env_t *) env_ptr;
  long hash = 0;
  for (int i = 0; i < GWIDTH; i++) {
    hash += env->elevation[i] * 17;
  }
  
  hash += env->block * 5381 + env->spin * 1793 + env->block_x * 137;
  
  return hash;
}

long compare_env(void *this, void *that) {
  env_t *env_this = (env_t *) this;
  env_t *env_that = (env_t *) that;
  for (int i = 0; i < GWIDTH; i++) {
    long diff = env_this->elevation[i] - env_that->elevation[i];
    if (diff != 0) return diff;
  }

 if (env_this->block != env_that->block) 
   return env_this->block - env_that->block;
  
 return (env_this->spin - env_that->spin) 
        + (env_this->block_x - env_that->block_x);
}


q_table *init_qtable() {
  return create_hashtable(&compare_env, &hash_env_state, Q_SIZE);  
}

bool insert_qtable(q_table *table, env_t *env_state, void *value) {
  return hash_insert(table, (void *) env_state, sizeof(env_t), value);
}

void *find_qtable(q_table *table, env_t *env_state) {
  return hash_find(table, (void *) env_state);
}

void free_qtable(q_table *table) {
  hash_table_itr *itr = get_hash_table_itr(table);

  while (hash_iterator_hasnext(itr)) {
    env_t env;
    hash_iterator_next(itr, &env);
  }
  free(itr);
  free_hashtable(table);
}
