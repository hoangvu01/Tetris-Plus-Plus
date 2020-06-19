#include "qlearn.h"

#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "qtable.h"
#include "env_defns.h"
#include "display.h"
#include "game.h"

#define randfloat() ((float) rand() / (float) RAND_MAX)

static void compute_env(env_t *env, state_t *curr);
static int get_optimal_move(q_data *data, env_t *env, double **actions);

static void evaluate_state(state_t *curr, q_data *data);
static void evaluate_grid(grid_t grid, q_state *curr);
static void evaluate_heights(state_t *game, q_state *curr);

static double update_score(q_data *data);
static void print_scr(q_state *curr);

const int actions_space[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'Z', 'X'};

typedef struct timespec timespec_t;

void updateFrame(timespec_t *now, timespec_t *lastFrame,
                 unsigned long *frameNum); 

int play(q_data *data) {
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
    step(data, curr);
    print_scr(data->prev);
    printState(curr, game_win);
    if (frameNum % framePerDrop(curr->level) == 0)
      hasMoving = dropPiece(curr);
  }
  free(data->curr->heights);
  free(data->curr);
  freeState(curr);
  return curr->level.score;
}

void updateFrame(timespec_t *now, timespec_t *lastFrame,
                 unsigned long *frameNum) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, now);
  double delta = (now->tv_sec - lastFrame->tv_sec) * 1e6 +
                 (now->tv_nsec - lastFrame->tv_nsec) / 1e3;
  *lastFrame = *now;
  if (delta < 1e6 / FRAME_RATE) {
    usleep(1e6 / FRAME_RATE - delta);
  }
  *frameNum = *frameNum + 1;
}

void simulate_input(state_t *curr, int key) {
  state_t teststate = *curr;
  switch (key) {
    case KEY_DOWN:
      teststate.pos.y++;
      break;
    case KEY_RIGHT:
      teststate.pos.x++;
      break;
    case KEY_LEFT:
      teststate.pos.x--;
      break;
    case 'Z':
    case 'z':
      teststate.rotation = antiClockwise(curr->block, teststate.rotation);
      break;
    case 'X':
    case 'x':
      teststate.rotation = clockwise(curr->block, teststate.rotation);
      break;
    case 'P':
    case 'p':
      pauseGame();
      break;
  }
  if (canMove(&teststate)) *curr = teststate;
}

static int get_optimal_move(q_data *data, env_t *env, double **actions) {
  double *q_actions = (void *) find_qtable(data->qtable, env);
  /* No action existing */
  if (q_actions == NULL) {
    q_actions = calloc(NO_ACTIONS, sizeof(double));
    insert_qtable(data->qtable, env, (void *) q_actions);
    *actions = q_actions;
    return 0;
  }

  /* Find best action */
  int move = 0;
  for (int i = 1; i < NO_ACTIONS; i++){
    if (q_actions[move] < q_actions[i]) move = i;
  } 

  *actions = q_actions;
  return move;
}

void step(q_data *data, state_t *curr) {
  int move;
  double **actions = malloc(sizeof(double *));
  
  /* Set up environment */
  env_t *env = calloc(1, sizeof(env_t));
  if (env == NULL) return;  
  compute_env(env, curr);
  

  /* Exploration or Exploitation */
  move = get_optimal_move(data, env, actions);
  if (randfloat() < EPSILON) 
    move = rand() % NO_ACTIONS; /* Exploration */

  /* Evaluate the move */
  compute_env(env, curr);
  data->curr = calloc(1, sizeof(q_state));
  data->curr->heights = calloc(GWIDTH, sizeof(int));
  simulate_input(curr, actions_space[move]);
  evaluate_state(curr, data);  
  
   
  /* Calculate Q-Value */
  double reward = update_score(data);
  double next_max = (*actions)[get_optimal_move(data, env, actions)];
  double new_score = data->curr->score + ALPHA * (reward + GAMMA * next_max);
  (*actions)[move] = new_score;
  data->curr->score = new_score;
 
  /* Swap previous frame with current frame */
  if (data->prev != NULL) {
    free(data->prev->heights);
    free(data->prev);
  }
  data->prev = data->curr;

  /* Free locally created resources */
  free(actions);
  free(env);
}

static void compute_env(env_t *env, state_t *curr) {
  env->block = curr->block - curr->list;
  env->spin = curr->rotation;
  env->block_x = curr->pos.x;
  env->elevation = calloc(GWIDTH, sizeof(int));

  int max_height = 0;
  for (int i = 0; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      if (curr->grid[i][j] > 0 && env->elevation[j] == 0) {
        env->elevation[j] = GHEIGHT - i;       
      }
      max_height = fmax(max_height, env->elevation[j]);
    }
  }
  for (int i = 0; i < GWIDTH; i++){
    env->elevation[i] -= max_height;
    if (env->elevation[i] <= -3) env->elevation[i] = -2;
  }
}

static void evaluate_state(state_t *curr, q_data *data) {
  evaluate_grid(curr->grid, data->curr);
  evaluate_heights(curr, data->curr);
}

static void evaluate_grid(grid_t grid, q_state *qstate) {
  for (int i = 0; i < GHEIGHT; i++) {
    bool line_completed = true; 
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] > 0) {
        /* update column height */
        if (qstate->heights[j] == 0) 
          qstate->heights[j] = GHEIGHT - i;       
      } else {
        line_completed = false;
        /* find holes in the tetris */
        if (i > GHEIGHT - qstate->heights[j]) 
          qstate->holes++;
      }
    }
    if (line_completed) qstate->complete_lines++;
  }
}

static void evaluate_heights(state_t *curr, q_state *qstate) {
  qstate->max_height = 0;
  for (int i = 0; i < GWIDTH; i++) {
    qstate->max_height = fmax(qstate->heights[i], qstate->max_height);
    qstate->aggr_height += pow(qstate->heights[i], 1.05);
    if (i > 0) 
     qstate->bumpiness += abs(qstate->heights[i] - qstate->heights[i - 1]);
  }
}

static double update_score(q_data *data) {
  q_state *curr = data->curr;
  q_state *prev = data->prev;
  double new_score = 
    A * curr->aggr_height + B * curr->bumpiness + 
    C * curr->complete_lines + D * curr->holes + 
    E * curr->max_height;

  double reward = 0;
  if (prev != NULL) {
    reward += curr->complete_lines * 50;
    if (prev->score <= curr->score) reward += 10;
    if (prev->holes >= curr->holes) reward += 30;
    if (prev->aggr_height >= curr->aggr_height) reward += 30;
    if (prev->bumpiness >= curr->bumpiness) reward += 15;
    if (prev->max_height >= curr->max_height) reward += 20;
  }

  curr->score = (1 - ALPHA) * new_score;
  return reward;
}

static void print_scr(q_state *curr) {
  wmove(stdscr, 100, 0);
  wprintw(stdscr, "aggr_height: %d\n", curr->aggr_height);
  wprintw(stdscr, "bumpiness: %d\n", curr->bumpiness);
  wprintw(stdscr, "complete_lines: %d\n", curr->complete_lines);
  wprintw(stdscr, "holes: %d\n", curr->holes);
  wprintw(stdscr, "max_height: %d\n", curr->max_height);
  wprintw(stdscr, "score: %lf\n", curr->score);
  wprintw(stdscr, "H: [");
  for (int i = 0; i < GWIDTH; i++)
    wprintw(stdscr, "%d ", curr->heights[i]);
  wprintw(stdscr, "]\n");
}

