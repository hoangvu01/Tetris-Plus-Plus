#include "qlearn.h"

#define LEVEL 500 

#define NO_ACTIONS 6

#define ALPHA 0.2
#define GAMMA 0.8
#define EPSILON 0.2

#define A (-0.281)
#define B (-0.132)
#define C (10)
#define D (-0.3)
#define E (-0.7)

#define randfloat() ((float) rand() / (float) RAND_MAX)

static void compute_env(env_t *env, state_t *curr);
static int get_optimal_move(q_data_t *data, env_t *env, double **actions);

static void evaluate_state(state_t *curr, q_data_t *data);
static void evaluate_grid(grid_t grid, q_state *curr);
static void evaluate_heights(state_t *game, q_state *curr);

static double update_score(q_data_t *data);
static void print_scr(q_state *curr);
static void print_out(q_state *curr);

static int translate_input();

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
  env_t *env = calloc(1, sizeof(env_t));
  if (env == NULL) return;  
  compute_env(env, curr);

  /* Exploration or Exploitation */
  move = get_optimal_move(data, env, actions);
  if (randfloat() < EPSILON) 
  move = rand() % NO_ACTIONS; /* Exploration */

  // move = translate_input();
  /* Evaluate the move */
  data->curr = calloc(1, sizeof(q_state));
  data->curr->heights = calloc(GWIDTH, sizeof(int));
  processInput(curr, actions_space[move]);
  evaluate_state(curr, data);  
   
  /* Calculate Q-Value */
  compute_env(env, curr);
  double reward = update_score(data);
  double next_max = (*actions)[get_optimal_move(data, env, actions)];
  double new_score = data->curr->score + ALPHA * (reward + GAMMA * next_max);
  (*actions)[move] = new_score;
  data->curr->score = new_score;
  if (data->prev != NULL) {
    free(data->prev->heights);
    free(data->prev);
  }
  data->prev = data->curr;
 
  free(actions);
  free(env);
}

static void compute_env(env_t *env, state_t *curr) {
  env->block = curr->block - curr->list;
  env->spin = curr->rotation;
  env->block_x = INT_MAX;
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
  
  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);

    if (cell.x >= 0 && cell.x < GWIDTH) 
      env->block_x = fmin(cell.x, env->block_x);
  }
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
  qstate->max_height = 0;
  for (int i = 0; i < GWIDTH; i++) {
    qstate->max_height = fmax(qstate->heights[i], qstate->max_height);
    qstate->aggr_height += pow(qstate->heights[i], 1.05);
    if (i > 0) 
     qstate->bumpiness += abs(qstate->heights[i] - qstate->heights[i - 1]);
  }
}

static double update_score(q_data_t *data) {
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

int translate_input() {
  int key = getch();
  switch (key) {
    case KEY_DOWN: return 1;
    case KEY_RIGHT: return 3;
    case KEY_LEFT: return 2;
    case 'z': return 4;
    case 'x': return 5;
    default: return 0;
  }
}

