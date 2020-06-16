#include "qlearn.h"

#define LEVEL 19 

#define NO_ACTIONS 6

#define ALPHA 0.1
#define GAMMA 0.6
#define EPSILON 0.15

#define A (-0.12)
#define B (-0.191)
#define C (10)
#define D (-0.6)

#define randfloat() ((float) rand() / (float) RAND_MAX)

static int get_optimal_move(q_data_t *data, env_t *env, double **actions);

static void evaluate_state(state_t *curr, q_data_t *data);
static void evaluate_grid(grid_t grid, q_state *curr);
static void evaluate_heights(state_t *game, q_state *curr);

static void update_score(q_data_t *data);
static void print_scr(q_state *curr);
// static void print_out(q_state *curr);

const int actions_space[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, 'Z', 'X'};


int play(q_data_t *data) {
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
  freeState(curr);
  return curr->level.score;
}

static int get_optimal_move(q_data_t *data, env_t *env, double **actions) {
  double *q_actions = find_qtable(data->qtable, env);
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

void step(q_data_t *data, state_t *curr) {
  int move;
  double **actions = malloc(sizeof(double *));
  
  /* Set up environment */
  env_t *env = malloc(sizeof(env_t));
  if (env == NULL) return;  
  env->block = curr->block;
  env->list = curr->list;
  env->grid = curr->grid;

  /* Exploration or Exploitation */
  move = get_optimal_move(data, env, actions);
  if (randfloat() < EPSILON) 
    move = rand() % NO_ACTIONS; /* Exploration */

  /* Evaluate the move */
  data->curr = calloc(1, sizeof(q_state));
  data->curr->heights = calloc(GWIDTH, sizeof(int));
  processInput(curr, actions_space[move]);
  evaluate_state(curr, data);  
  update_score(data);
   
  /* Calculate Q-Value */ 
  double next_max = (*actions)[get_optimal_move(data, env, actions)];
  double reward = (data->prev == NULL ? 0 : data->prev->score) - data->curr->score;
  double new_score = (1 - ALPHA) * data->curr->score + 
                      ALPHA * (reward + GAMMA * next_max);
  (*actions)[move] = new_score;
  data->curr->score = new_score;
  if (data->prev != NULL) {
    free(data->prev->heights);
    free(data->prev);
  }
  data->prev = data->curr;
  data->prev->score = new_score;

  free(actions);
}

static void evaluate_state(state_t *curr, q_data_t *data) {
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
  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);

    if (cell.x >= 0 && cell.x < GWIDTH) qstate->heights[cell.x]++;
  }

  for (int i = 0; i < GWIDTH; i++) {
    qstate->aggr_height += sqrt(qstate->heights[i]);
    if (i > 0) 
     qstate->bumpiness += abs(qstate->heights[i] - qstate->heights[i - 1]);
  }
}

static void update_score(q_data_t *data) {
  q_state *curr = data->curr;
  q_state *prev = data->prev;
  double new_score = 
    A * curr->aggr_height + B * curr->bumpiness + 
    C * curr->complete_lines + D * curr->holes;

  if (prev != NULL) {
    if (prev->score <= curr->score) new_score += 10;
    if (prev->holes >= curr->holes) new_score += 6;
    if (prev->aggr_height >= curr->aggr_height) new_score += 10;
    if (prev->bumpiness >= curr->bumpiness) new_score += 2;
  }

  curr->score = new_score;

}
/*
void print_out(q_state *curr) {
  fprintf(stdout, "aggr_height: %d\n", curr->aggr_height);
  fprintf(stdout, "bumpiness: %d\n", curr->bumpiness);
  fprintf(stdout, "complete_lines: %d\n", curr->complete_lines);
  fprintf(stdout, "holes: %d\n", curr->holes);
  fprintf(stdout, "score: %lf\n", curr->score);
  fprintf(stdout, "H: [");
  for (int i = 0; i < GWIDTH; i++)
    fprintf(stdout, "%d ", curr->heights[i]);
  fprintf(stdout, "]\n");
}
*/

static void print_scr(q_state *curr) {
  wmove(stdscr, 100, 0);
  wprintw(stdscr, "aggr_height: %d\n", curr->aggr_height);
  wprintw(stdscr, "bumpiness: %d\n", curr->bumpiness);
  wprintw(stdscr, "complete_lines: %d\n", curr->complete_lines);
  wprintw(stdscr, "holes: %d\n", curr->holes);
  wprintw(stdscr, "score: %lf\n", curr->score);
  wprintw(stdscr, "H: [");
  for (int i = 0; i < GWIDTH; i++)
    wprintw(stdscr, "%d ", curr->heights[i]);
  wprintw(stdscr, "]\n");
}


