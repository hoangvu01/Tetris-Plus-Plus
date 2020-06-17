#ifndef ENV_H
#define ENV_H

#include "hash_table.h"

#include "game.h"
#include "state.h"
#include "grid.h"
#include "display.h"


typedef hash_table q_table;

typedef struct {
  int block;
  int spin;
  int *elevation; /* Relative to the first column */
} env_t;

typedef struct {
  int *heights;
  int aggr_height;
  int bumpiness;
  int complete_lines;
  int holes;
  double score;
} q_state;

typedef struct {
  q_table *qtable;
  q_state *prev;
  q_state *curr;
} q_data_t;

#endif
