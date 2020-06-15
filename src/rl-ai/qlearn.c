#include "qlearn.h"

#define LEVEL 1 

#define NO_ACTIONS 6
#define ALPHA (-0.1)
#define BETA  (0.1)
#define GAMMA (-0.1)
#define DELTA (-0.1)

static void step(q_data_t *data, state_t *curr);

const int actions_space[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'Z', 'X'};



void to_binary_grid(grid_t *grid) {
  for (int i = 0; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      *grid[i][j] = *grid[i][j] > 0;
    }
  }
}


void evaluate_grid(grid_t grid, int *heights, double *score) {
  int hole_penalty, complete_line_reward;
  for (int i = 0; i < GHEIGHT; i++) {
    bool line_completed = true; 
    for (int j = 0; j < GWIDTH; j++) {
      /* update column height */
      if (grid[i][j] > 0 && heights[j] == 0){
        heights[j] = GHEIGHT - i;       
      }  
      if (grid[i][j] == 0) {
        line_completed = false;
        if (i > GHEIGHT - heights[j]) hole_penalty++;
      }
    }
    if (line_completed) complete_line_reward += 10;
  }

  int aggr_height = heights[0];
  int bumpiness = 0;
  for (int i = 1; i < GWIDTH; i++) {
    aggr_height += heights[i];
    bumpiness += abs(heights[i] - heights[i - 1]);
  }
 
  wmove(stdscr, 50, 5); 
  wprintw(stdscr, "\n");
  wprintw(stdscr, "Holes: %d\n", hole_penalty);
  wprintw(stdscr, "CL: %d\n", complete_line_reward);
  wprintw(stdscr, "BP: %d\n", bumpiness);
  wprintw(stdscr, "AGG_H: %d\n", aggr_height);
  wprintw(stdscr, "H: [");
  for (int i = 0; i < GWIDTH; i++)
    wprintw(stdscr, "%d ", heights[i]);
  wprintw(stdscr, "]\n");  

  *score = ALPHA * hole_penalty + BETA * complete_line_reward + 
           GAMMA * bumpiness + DELTA * aggr_height;
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
    step(data, curr);
    if (frameNum % framePerDrop(curr->level) == 0)
      hasMoving = dropPiece(curr);
  }
  
  freeState(curr);
  endwin();
  
  if (curr->level.score > curr->highScore) writeHighScore(curr->level.score);
  printf("You scored %d points. \n", curr->level.score);
  printf("The high score is %d points. \n", readHighScore());
  
}

static void step(q_data_t *data, state_t *curr) {
  float random = (float) rand() / (float) RAND_MAX;
  int move = 0;
  if (random > 0) move = make_action(data, curr);
  
  processInput(curr, move);
}

int make_action(q_data_t *data, state_t *curr) {
  double score;
  int *heights = calloc(GWIDTH, sizeof(int));
  evaluate_grid(curr->grid, heights, &score);  
  wprintw(stdscr, "\nCurrent evaluation: %f\n", score);
  return actions_space[rand() % NO_ACTIONS];
}

