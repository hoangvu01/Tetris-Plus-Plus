#ifndef ENV_H
#define ENV_H

#include "hash_table.h"

#define LEVEL 1000 
#define FRAME_RATE 60

#define NO_ACTIONS 6

#define ALPHA 0.2
#define GAMMA 0.8
#define EPSILON 0.2

#define A (-0.35)
#define B (-0.132)
#define C (10)
#define D (-0.3)
#define E (-0.7)

typedef hash_table q_table;

typedef struct {
  int block;
  int block_x;
  int spin;
  int *elevation; /* Relative to the first column */
} env_t;

typedef struct {
  int *heights;
  int aggr_height;
  int bumpiness;
  int complete_lines;
  int holes;
  int max_height;
  double score;
} q_state;

typedef struct {
  q_table *qtable;
  q_state *prev;
  q_state *curr;
} q_data;

#endif
