

#ifndef Q_LEARN_H
#define Q_LEARN_H

#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "qtable.h"
#include "env_defns.h"
#include "display.h"
#include "game.h"

/* Start game */
int play(q_data *data);

/* Performs an action given state of the game */
void step(q_data *, state_t *);


#endif
