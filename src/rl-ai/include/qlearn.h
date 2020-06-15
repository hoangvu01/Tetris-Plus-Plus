

#ifndef Q_LEARN_H
#define Q_LEARN_H

#include <time.h>
#include <unistd.h>

#include "game.h"
#include "state.h"
#include "grid.h"
#include "hash_table.h"
#include "display.h"


typedef struct {
  long reward; 
  state_t env;
} q_data_t;

void play();

int make_action(q_data_t *, state_t *);


#endif
