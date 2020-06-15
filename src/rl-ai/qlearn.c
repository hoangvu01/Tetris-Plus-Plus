#include "qlearn.h"

#define LEVEL 10

#define NO_ACTIONS 6

const int actions_space[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'Z', 'X'};

void to_binary_grid(grid_t grid) {
  for (int i = 0; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      grid[i][j] = grid[i][j] > 0;
    }
  }
}

void evaluate_grid(grid_t grid, int *penalty) {
  int hole_penalty, height_penalty = GHEIGHT;
  
  for (int i = 0; i < GHEIGHT; i++) {
    height_penalty--;
    for (int j = 0; j < GWIDTH; j++) {
        
    }
  }
}

void play(q_data_t *data) {
  WINDOW *game_win = init_display();
  state_t *curr = initState(LEVEL);
  bool hasMoving = false;

  timespec_t now, lastFrame;
  unsigned long frameNum = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);
 
  while(1) {
    updateFrame(&now, &lastFrame, &frameNum);

    if (!hasMoving) {
      if (!spawnTetriminos(curr)) break;
      hasMoving = true;
    }
    printState(curr, game_win); 
    int action = make_action(data, curr);
    processInput(curr, action);

    if (frameNum % framePerDrop(curr->level) == 0)
      hasMoving = dropPiece(curr);
  }
  
  freeState(curr);
  endwin();
  
  if (curr->level.score > curr->highScore) writeHighScore(curr->level.score);
  printf("You scored %d points. \n", curr->level.score);
  printf("The high score is %d points. \n", readHighScore());
  
}

int step(q_data_t *data, state_t *curr) {
  return KEY_DOWN;
}

int make_action(q_data_t *data, state_t *curr) {
  return actions_space[rand() % NO_ACTIONS];
}

