#include "tetris.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <unistd.h>

#include "grid.h"
#include "tetriminos.h"

static tetrimino_t *list;
static grid_t grid;
void detectInput(tetrimino_t *block, position_t *pos, int *rotation);
bool canMove(tetrimino_t *block, position_t pos, int rotation);
void printState(tetrimino_t *block, position_t pos, int rotation);
int clearLines();

int main(int argc, char const *argv[]) {
  WINDOW *w = initscr();
  cbreak();
  nodelay(w, TRUE);
  noecho();

  grid = initGrid();
  list = initTetrimino();
  tetrimino_t *curr;
  int rotation;
  position_t pos;
  bool hasMoving = false;

  struct timespec now, lastFrame;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);
  unsigned long frameNum = 0;

  while (1) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    double delta = (now.tv_sec - lastFrame.tv_sec) * 1e6 + (now.tv_nsec - lastFrame.tv_nsec) / 1e3; 
    lastFrame = now;
    frameNum++;
    if (delta < 16667) {
      usleep(16667 - delta);
    }

    if (!hasMoving) {
      curr = list + rand() % 7;
      pos.x = 5;
      pos.y = 2;
      rotation = 0;
      if (!canMove(curr, pos, rotation)) break;
      hasMoving = true;
    }
    printState(curr, pos, rotation);
    detectInput(curr, &pos, &rotation);

    /* gravity */
    if (frameNum % 28 == 0) {
      position_t testpos = pos;
      testpos.y++;
      if (canMove(curr, testpos, rotation)) {
        pos = testpos;
      } else {
        for (int i = 0; i < 4; i++) {
          position_t cell;
          pplus(&cell, pos, curr->spins[rotation][i]);
          *(getSquare(grid, cell)) =
              curr - list + 1;  // set colour to block val
        }
        clearLines();
        hasMoving = false;
      }
    }

  }

  endwin();
  freeGrid(grid);
  freeTetriminos(list);
  return EXIT_SUCCESS;
}

int clearLines() {
  int completedlines = 0;
  /* clears top buffers */
  for (int i = 0; i < 2; i++) {
    memset(grid[i], 0x00, GWIDTH * sizeof(colour_t));
  }

  for (int i = 2; i < GHEIGHT; i++) {
    bool isComplete = true;
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] == 0) isComplete = false;
    }

    if (isComplete) {
      completedlines++;
      for (int k = i; k >= 2; k--) {
        memcpy(grid[k], grid[k - 1], GWIDTH * sizeof(colour_t));
      }
    }
  }
  return completedlines;
}

void printState(tetrimino_t *block, position_t pos, int rotation) {
  grid_t output = cloneGrid(grid);

  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, pos, block->spins[rotation][i]);
    *(getSquare(output, cell)) = block - list + 1;  // set colour to block val
  }
  clear();
  printGrid(output);
  refresh();
  freeGrid(output);
}

void detectInput(tetrimino_t *block, position_t *pos, int *rotation) {
  position_t testpos = *pos;
  int testrotate = *rotation;
  switch (getch()) {
    case '\033':
      getch();  // skip the [
      switch (getch()) {
        case 'B':  // down
          testpos.y++;
          break;
        case 'C':  // right
          testpos.x++;
          break;
        case 'D':  // left
          testpos.x--;
          break;
      }
      break;
    case 'Z':
    case 'z':
      testrotate = antiClockwise(block, testrotate);
      break;
    case 'X':
    case 'x':
      testrotate = clockwise(block, testrotate);
      break;
  }
  if (!canMove(block, testpos, testrotate)) return;
  *pos = testpos;
  *rotation = testrotate;
}

bool canMove(tetrimino_t *block, position_t pos, int rotation) {
  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, pos, block->spins[rotation][i]);

    if (cell.y < -2 || cell.y >= GHEIGHT) return false;
    if (cell.x < 0 || cell.x >= GWIDTH) return false;
    if (*getSquare(grid, cell) != 0) return false;
  }
  return true;
}