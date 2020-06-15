#include "qlearn.h"

#define LEVEL 1 

#define NO_ACTIONS 6


#define ALPHA 0.1
#define GAMMA 0.6
#define EPSILON 0.1

#define A (-0.01)
#define B  (0.5)
#define C (-0.01)
#define D (-0.01)
#define E (0.1)

static void evaluate_grid(grid_t grid, int *heights, double *score);
static void evaluate_heights(state_t *curr, int *heights, double *score);

const int actions_space[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'Z', 'X'};

void evaluate_state(state_t *curr, int *heights, double *score) {
  evaluate_grid(curr->grid, heights, score);
  evaluate_heights(curr, heights, score);
}

void evaluate_grid(grid_t grid, int *heights, double *score) {
  int hole_penalty = 0, complete_line = 0;
  for (int i = 0; i < GHEIGHT; i++) {
    bool line_completed = true; 
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] > 0) {
        /* update column height */
        if (heights[j] == 0) heights[j] = GHEIGHT - i;       
      } else {
        line_completed = false;
        /* find holes in the tetris */
        if (i > GHEIGHT - heights[j]) hole_penalty++;
      }
    }
    if (line_completed) complete_line++;
  }

  wmove(stdscr, 50, 5); 
  wprintw(stdscr, "\n");
  wprintw(stdscr, "Holes: %d\n", hole_penalty);
  wprintw(stdscr, "CL: %d\n", complete_line);
  
  *score = A * hole_penalty + B * complete_line; 
}

void evaluate_heights(state_t *curr, int *heights, double *score) {
  int bumpiness = 0, aggr_height = 0;
  for (int i = 0; i < GWIDTH; i++) {
    aggr_height += heights[i];
    if (i > 0) bumpiness += abs(heights[i] - heights[i - 1]);
  }

  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);

    if (cell.x >= 0 && cell.x < GWIDTH) {
      heights[cell.x]++;
    }
  }

  wprintw(stdscr, "H: [");
  for (int i = 0; i < GWIDTH; i++) wprintw(stdscr, "%d ", heights[i]);
  wprintw(stdscr, "]\n");

  *score += C * aggr_height + D * bumpiness;
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


void step(q_data_t *data, state_t *curr) {
  int move, *actions;
  double score;
  
  float random = (float) rand() / (float) RAND_MAX;
  env_t *env = malloc(sizeof(env_t));
  env->block = curr->block;
  env->list = curr->list;
  env->grid = curr->grid;
 
  actions = find_qtable(data->qtable, env);
  if (random < EPSILON || actions == NULL) {
    actions = malloc(sizeof(int) * NO_ACTIONS);
    for (int i = 0; i < NO_ACTIONS; i++)
      actions[i] = INT_MIN;
    move = rand() % NO_ACTIONS;
  } else {
    move = actions[0]; 
    for (int i = 1; i < NO_ACTIONS; i++){
      if (move < actions[i]) move = i;
    }  
  }
  
  processInput(curr, actions_space[move]);

  int *heights = calloc(GWIDTH, sizeof(int));
  evaluate_state(curr, heights, &score);  
  wprintw(stdscr, "Score: %f\n", score);
  if (data->prev_score == curr->level.score) {
    score -= 0.01;
  }
  
  actions[move] = score;
  free(heights);
}

