

#ifndef Q_LEARN_H
#define Q_LEARN_H

#include "env_defns.h"
#include "state.h"

/* Start game */
int play(q_data *data);

/* Performs an action given state of the game */
void step(q_data *, state_t *);


#endif
