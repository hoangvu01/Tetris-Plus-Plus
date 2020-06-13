#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FRAME_RATE 60
typedef struct timespec timespec_t;
void updateFrame(timespec_t *now, timespec_t *lastFrame, unsigned long *frameNum);