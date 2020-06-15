

#ifndef Q_LEARN_H
#define Q_LEARN_H

#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "qtable.h"
#include "env_defns.h"


void play(q_data_t *data);

void step(q_data_t *, state_t *);


#endif
