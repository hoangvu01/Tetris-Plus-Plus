#include "qtable.h"

#define Q_SIZE 100

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

int hash_env_state(void *ptr) {
  env_t *data = (env_t *) ptr;
  int hash = 0;
  for (int i = 0; i < GHEIGHT; i++){
    for (int j = 0; j < GWIDTH; j++) {
      hash *= i * GWIDTH + j;
      hash += data->grid[i][j] > 0;
    }
  }
  
  hash += (data->block - data->list) * 5381;
  return hash;
}

q_table *init_qtable() {
  return create_hashtable(&compare_env_state, &hash_env_state, Q_SIZE);  
}

bool insert_qtable(q_table *table, env_t *env_state, void *value) {
  return hash_insert(table, (void *) env_state, sizeof(env_t), value);
}

void *find_qtable(q_table *table, env_t *env_state) {
  return hash_find(table, (void *) env_state);
}
