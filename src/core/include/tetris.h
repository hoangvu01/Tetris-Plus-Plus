#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FRAME_RATE 60
typedef struct timespec timespec_t;

/*
 * waits until the timing is right, then advances the current frame by 1. User
 * input is detected on every frame, print_state happens every frame, but the
 * piece only drops on specific frames.  
 */
void updateFrame(timespec_t *now, timespec_t *lastFrame,
                 unsigned long *frameNum);