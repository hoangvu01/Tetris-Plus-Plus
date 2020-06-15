
#ifndef GAME_H
#define GAME_H


#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "display.h"
#include "levels.h"
#include "state.h"

#define FRAME_RATE 60


typedef struct timespec timespec_t;

void startGame(int levelNum);

void updateFrame(timespec_t *now, timespec_t *lastFrame, unsigned long *frameNum);

#endif
