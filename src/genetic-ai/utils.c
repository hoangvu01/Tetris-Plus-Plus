#include <string.h>
#include "utils.h"

/* The static functions are only used in this file. */

double cap_to_nonnegative(double n) {
    if (n < 0) return 0.0;
    else return n;
}

block_t* init_block_from_state(const state_t *state) {
    block_t *block = malloc(sizeof(block_t));
    block->pos = state->pos;
    block->rotation = state->rotation;
    block->id = state->block - state->list;
    assert(block->id >= 0);
    assert(block->id < NUM_BLOCKS);
    return block;
}

block_t* clone_block(const block_t* block) {
    block_t* new_block = malloc(sizeof(block_t));
    *new_block = *block;
    return new_block;
}

void free_block(block_t *block) {
    free(block);
}

void set_state_by_block(state_t *state, const block_t *block) {
    state->pos = block->pos;
    state->rotation = block->rotation;
    state->block = state->list + block->id;
}

void print_block(const block_t *block) {
    if (block == NULL) printf("block_t(NULL)");
    else printf("block_t(id=%d, pos=%d_%d, rotation=%d)\n", block->id, block->pos.x, block->pos.y, block->rotation);
}


void normalize(param_state_t *param) {
    /* temporary variables set to register in order to run faster */
  register double hw = param->aggregate_height_w;
  hw *= hw;
  register double lw = param->complete_line_w;
  lw *= lw;
  register double nw = param->hole_number_w;
  nw *= nw;
  register double bw = param->bumpiness_w;
  bw *= bw;
  double magnitude = sqrt(hw + lw + nw + bw);

  param->aggregate_height_w /= magnitude;
  param->complete_line_w /= magnitude;
  param->hole_number_w /= magnitude;
  param->bumpiness_w /= magnitude;
  //param->risk_w /= magnitude;
}

bool dropPieceWithOptions(state_t *curr, bool clear_lines, bool color_grid, int* lines_cleared) {
  state_t teststate = *curr;
  teststate.pos.y++;
  if (canMove(&teststate)) {
    *curr = teststate;
    return true;
  } else {
    if (color_grid) {
      for (int i = 0; i < 4; i++) {
        position_t cell;
        pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);
        *(getSquare(curr->grid, cell)) =
            curr->block - curr->list + 1;  // set colour to block val
      }
    }
    if (clear_lines) {
      int linesCleared = clearLines(curr->grid);
      if (lines_cleared != NULL) *lines_cleared = linesCleared;
      curr->totalLines += linesCleared;
      curr->level = update_level(curr->level, curr->totalLines, linesCleared);
    }
    return false;
  }
}

void free_param(param_state_t *param) {
  if (param == NULL) {
    perror("free_param(): Failed to free the param pointer");
    exit(EXIT_FAILURE);
  }
  free(param);
}

void free_param_array(param_state_t **param_array) {
  if (param_array) {
    perror("free_param_array(): Failed to free the array of parameter vectors");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; param_array[i] != NULL; i++) {
    free_param(param_array[i]);
  }
  free(param_array);
}

void print_param(const param_state_t *param) {
    printf("param_state_t(height=%f, clear_lines=%f, hole_num=%f, bumpiness=%f, loss=%d)\n", param->aggregate_height_w, param->complete_line_w, param->hole_number_w, param->bumpiness_w, param->loss);
}

void print_param_array(param_state_t **param_array, int array_size) {
    printf("==================\n");
    for (int i = 0; i < array_size; i++) {
        print_param(param_array[i]);
    }
}

static int _sort_param_array_cmp(const void *a, const void *b) {
    return ((*(param_state_t **) b)->loss - (*(param_state_t **) a)->loss);
}

void sort_param_array(param_state_t **param_array, int array_size) {
  qsort(param_array, array_size, sizeof(param_state_t*), _sort_param_array_cmp);
}

static int get_column_height(grid_t grid, int c) {
    for (int y = 2; y < GHEIGHT; y++) {
        if (grid[y][c] != 0) {
            return GHEIGHT - y;
        }
    }

    return 0;
}

int get_aggregate_height(grid_t grid) {
    int h = 0;
    for (int x = 0; x < GWIDTH; x++) {
        h += get_column_height(grid, x);
    }

    return h;
}

int get_complete_line(grid_t grid) {
    int completedlines = 0;
    for (int i = 2; i < GHEIGHT; i++) {
        bool isComplete = true;
        for (int j = 0; j < GWIDTH; j++) {
            if (grid[i][j] == 0) {
                isComplete = false;
                break;
            }
        }

        if (isComplete) completedlines++;
    }
    return completedlines;
}

int get_hole_number(grid_t grid) {
    int n = 0;
    for (int x = 0; x < GWIDTH; x++) {
        bool is_block = false;
        for (int y = 2; y < GHEIGHT; y++) {
            if (is_block) n++;
            else if (grid[y][x] != 0) is_block = true;
        }
    }

    return n;
}
int get_bumpiness(grid_t grid) {
    int b = 0;
    for (int x = 0; x < GWIDTH - 1; x++) {
        b += abs(get_column_height(grid, x) - get_column_height(grid, x+1));
    }
    return b;
}
/* This function is originally bnuilt for increasing AI's will to take risks, but turned out to be ineffective.
 * The related fields/functions to risk is disabled. The current increase of will to take risk is due to different implementation in best_move
int get_risk(grid_t grid) {
  int risk = 0;
  if (get_column_height(grid, 1) - get_column_height(grid, 0) <= 5) {
    risk += 4;
  }
  for (int x = 1; x < GWIDTH - 1; x++) {
    int diff1 = get_column_height(grid, x-1) - get_column_height(grid, x);
    int diff2 = get_column_height(grid, x) - get_column_height(grid, x+1);
    if (diff1 >= 5 && diff2 <= -5) {
        risk += 4;
    }
  }
  if (get_column_height(grid, GWIDTH - 2) - get_column_height(grid, GWIDTH - 1) <= 5) {
    risk += 4;
  }

  return risk;
}
*/
void print_and_save_result(param_state_t **array, bool is_saving) {
    param_state_t *first = array[0];
    param_state_t *second = array[1];
    printf("The first fittest param: loss - %d, aggregate_height_w - %f, complete_line_w - %f, hole_number_w - %f, bumpiness_w - %f\n",
            first->loss, first->aggregate_height_w, first->complete_line_w, first->hole_number_w, first->bumpiness_w);
    printf("The second fittest param: loss - %d, aggregate_height_w - %f, complete_line_w - %f, hole_number_w - %f, bumpiness_w - %f\n",
           second->loss, second->aggregate_height_w, second->complete_line_w, second->hole_number_w, second->bumpiness_w);
    if (is_saving) {
        FILE *fp = fopen("training_progress.txt", "w");
        //fprintf(fp, "loss, aggregate_height_w, complete_line_w, hole_number_w, bumpiness_w\n");
        for (int i = 0; array[i] != NULL; i++) {
            fprintf(fp, "%d %f %f %f %f\n", array[i]->loss, array[i]->aggregate_height_w,
                    array[i]->complete_line_w, array[i]->hole_number_w, array[i]->bumpiness_w);
        }
        fclose(fp);
    }
}

param_state_t **read_param_from_file(char *filename, int array_size) {
  FILE *fp = fopen(filename, "r");

  /*if (fgets(str, 80, fp) == NULL || !(strcmp(str, "loss, aggregate_height_w, complete_line_w, hole_number_w, bumpiness_w") == 0)) {
    perror("read_param_from_file(): The file format is incorrect!");
    exit(EXIT_FAILURE);
  }*/

  param_state_t **param_array = calloc(array_size + 1, sizeof(param_state_t *));
  for (int i = 0; i < array_size; i++) {
    param_array[i] = malloc(sizeof(param_state_t));
  }
  param_array[array_size] = NULL;
  int i = 0;
  while (fscanf(fp, "%d %lf %lf %lf %lf", &param_array[i]->loss, &param_array[i]->aggregate_height_w, &param_array[i]->complete_line_w, &param_array[i]->hole_number_w, &param_array[i]->bumpiness_w) == 5) {
    i++;
  }

  fclose(fp);

  return param_array;
}
