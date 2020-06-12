#include "tetris.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "tetriminos.h"

static tetrimino_t *list;
static grid_t grid;
void detectInput(tetrimino_t *block, position_t *pos, int *rotation);
bool canMove(tetrimino_t *block, position_t pos, int rotation);
void printState(tetrimino_t *block, position_t pos, int rotation);

int main(int argc, char const *argv[]) {
  initscr();
  timeout(-1);

  grid = initGrid();
  list = initTetrimino();
  tetrimino_t *curr;
  int rotation;
  position_t pos;
  bool hasMoving;

  while (1) {
    if (!hasMoving) {
      curr = list + (rand() % 7);
      pos.x = 5;
      pos.y = 2;
      rotation = 0;
      if (!canMove(curr, pos, rotation)) break;
      hasMoving = true;
    }

    detectInput(curr, &pos, &rotation);

    /* gravity */
    position_t testpos = pos;
    testpos.y++;
    if (canMove(curr, testpos, rotation)) {
      pos = testpos;
    } else {
      for (int i = 0; i < 4; i++) {
        position_t cell;
        pplus(&cell, pos, curr->spins[rotation][i]);
        *(getSquare(grid, cell)) = curr - list;  // set colour to block val
      }
      hasMoving = false;
    }

    printState(curr, pos, rotation);
  }

  endwin();
  freeGrid(grid);
  freeTetriminos(list);
  return EXIT_SUCCESS;
}

void printState(tetrimino_t *block, position_t pos, int rotation) {
  for (int i = 2; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      bool isMoving = false;
      for (int k = 0; k < 4; k++) {
        position_t cell;
        pplus(&cell, pos, block->spins[rotation][k]);
        if (cell.x == j && cell.y == i){
					printf("%d", (int) (block - list));
					isMoving = true;
				}
      }
      if (!isMoving) printf("%d", grid[i][j]);
    }
    printf("\n");
  }
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
      testrotate = clockwise(block, testrotate);
      break;
    case 'X':
      testrotate = antiClockwise(block, testrotate);
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

    if (cell.y < -2 || cell.y >= 20) return false;
    if (cell.x < 0 || cell.x >= 10) return false;
    if (getSquare(grid, cell) != 0) return false;
  }
  return true;
}