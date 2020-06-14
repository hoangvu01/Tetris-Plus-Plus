#include "state.h"
#include "display.h"

state_t *initState(int levelNum) {
  state_t *curr = malloc(sizeof(state_t));
  curr->grid = initGrid();
  curr->list = initTetrimino();
  curr->level = initLevel(levelNum);
  curr->totalLines = 0;
  curr->nextBlock = curr->list + rand() % 7;
  return curr;
}

void freeState(state_t *curr) {
  freeGrid(curr->grid);
  freeTetriminos(curr->list);
  free(curr);
}

bool spawnTetriminos(state_t *curr) {
  curr->block = curr->nextBlock;
  curr->nextBlock = curr->list + rand() % 7;

  curr->pos.x = 5;
  curr->pos.y = 2;
  curr->rotation = 0;
  if (!canMove(curr)) return false;
  return true;
}


bool dropPiece(state_t *curr) {
  state_t teststate = *curr;
  teststate.pos.y++;
  if (canMove(&teststate)) {
    *curr = teststate;
    return true;
  } else {
    for (int i = 0; i < 4; i++) {
      position_t cell;
      pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);
      *(getSquare(curr->grid, cell)) =
          curr->block - curr->list + 1;  // set colour to block val
    }
    int linesCleared = clearLines(curr->grid);
    curr->totalLines += linesCleared;
    curr->level = update_level(curr->level, curr->totalLines, linesCleared);
    return false;
  }
}

void detectInput(state_t *curr) {
  state_t teststate = *curr;
  switch (getch()) {
    case '\033':
      getch();  // skip the [
      switch (getch()) {
        case 'B':  // down
          teststate.pos.y++;
          break;
        case 'C':  // right
          teststate.pos.x++;
          break;
        case 'D':  // left
          teststate.pos.x--;
          break;
      }
      break;
    case 'Z':
    case 'z':
      teststate.rotation = antiClockwise(curr->block, teststate.rotation);
      break;
    case 'X':
    case 'x':
      teststate.rotation = clockwise(curr->block, teststate.rotation);
      break;
  }
  if (canMove(&teststate)) *curr = teststate;
}

bool canMove(state_t *test) {
  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, test->pos, test->block->spins[test->rotation][i]);

    if (cell.y < -2 || cell.y >= GHEIGHT) return false;
    if (cell.x < 0 || cell.x >= GWIDTH) return false;
    if (*getSquare(test->grid, cell) != 0) return false;
  }
  return true;
}
