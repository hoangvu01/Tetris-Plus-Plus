#ifndef ENV_H
#define ENV_H

#include "hash_table.h"

#include "game.h"
#include "state.h"
#include "grid.h"
#include "display.h"


typedef hash_table q_table;

typedef struct {
  tetrimino_t *block;
  tetrimino_t *list;
  grid_t grid;
} env_t;

typedef struct {
  q_table *qtable;
  double prev_score;
} q_data_t;

#endif
