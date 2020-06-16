#include "state.h"
#include "display.h"
#include "gpio_input.h"

state_t *initState(int levelNum) {
  state_t *curr = malloc(sizeof(state_t));
  curr->grid = initGrid();
  curr->list = initTetrimino();
  curr->level = initLevel(levelNum);
  curr->totalLines = 0;
  curr->nextBlock = curr->list + rand() % 7;
  curr->highScore = readHighScore();
  return curr;
}

state_t *cloneState(const state_t *state) {
    state_t *new_state = malloc(sizeof(state_t));
    memcpy(new_state, state, sizeof(state_t));
    new_state->grid = cloneGrid(state->grid);
    new_state->list = initTetrimino();
    new_state->nextBlock = new_state->list + (state->nextBlock - state->list);
    new_state->block = new_state->list + (state->block - state->list);
    return new_state;
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

operator_t getInput(inmode_t mode){
  if (mode == KEY_BOARD) {
    switch (getch()) {
      case KEY_DOWN:
        return DOWN;
      case KEY_LEFT:
        return LEFT;
      case KEY_RIGHT:
        return RIGHT;
      case 'Z':
      case 'z':
        return RLEFT;
      case 'X':
      case 'x':
        return RRIGHT;
      case 'P':
      case 'p':
        return PAUSE;        
      default:
        return NONE;
    }
  }
  #ifdef PI_MODE
    return gpio_input(mode);
  #endif
  fprintf(stderr, "Not in Raspberry PI mode.");
  return NONE;
}

void processInput(state_t *curr, inmode_t mode) {
  state_t state = *curr;
  switch (getInput(mode)) {
    case DOWN:
      state.pos.y++;
      break;
    case RIGHT:
      state.pos.x++;
      break;
    case LEFT:
      state.pos.x--;
      break;
    case RLEFT:
      state.rotation = antiClockwise(curr->block, state.rotation);
      break;
    case RRIGHT:
      state.rotation = clockwise(curr->block, state.rotation);
      break;
    case PAUSE:
      pauseGame();
      break;
    default:
      break;  
  }
  if (canMove(&state)) *curr = state;
}

void pauseGame() {
  timeout(-1);
  clear();
  char *message = "Press any key to continue game.";
  mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - strlen(message)) / 2,
           message);
  refresh();
  getch();
  nodelay(stdscr, true);
  clear();
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

int readHighScore(){
  FILE *file = fopen("highscore.txt", "r");
  if (!file) {
    return 0;
  }

  int val;
  fscanf(file, "%d", &val);
  if (ferror(file)) return 0;

  fclose(file);
  return val;
}

void writeHighScore(int highScore){
  FILE *file = fopen("highscore.txt", "w");
  if (!file) {
    fprintf(stderr, "write error. ");
  }
  fprintf(file, "%d", highScore);
}
