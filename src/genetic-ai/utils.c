#include "utils.h"
#include "state.h"

void normalize(param_state_t *param) {
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
}

param_state_t *generate_random_param() {
  param_state_t *param = malloc(sizeof(param_state_t));
  if (param == NULL) {
    perror("generate_random_param: Failed to generate random parameters");
    exit(EXIT_FAILURE);
  }
  param->aggregate_height_w = randomDouble(0, 1);
  param->complete_line_w = randomDouble(0, 1);
  param->hole_number_w = randomDouble(0, 1);
  param->bumpiness_w = randomDouble(0, 1);
  param->loss = (double) INT_MAX;

  return param;
}

void free_param(param_state_t *param) {
  if (param == NULL) {
    perror("free_param(): Failed to free the param pointer");
    exit(EXIT_FAILURE);
  }
  free(param);
}

param_state_t **init_param_array(int size) {
  param_state_t **array = calloc(size + 1, sizeof(param_state_t *));
  for (int i = 0; i < size; i++) {
    array[i] = generate_random_param();
  }
  array[size] = NULL;

  return array;
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
static void merge(param_state_t param_array[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    param_state_t L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = param_array[l + i];
    for (int j = 0; j < n2; j++) R[j] = param_array[m + j + 1];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->loss <= R[j]->loss) {
            param_array[k] = L[i];
            i++;
        } else {
            param_array[k] = R[j];
            j++
        }
        k++;
    }

    while (i < n1) {
        param_array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        param_array[k] = R[j];
        j++;
        k++;
    }
}

void sort_param_array(param_state_t **param_array, int l, int r) {
  if (l < r) {
      int m = l + (r - l) / 2;
      mergeSort(param_array, l, m);
      mergeSort(param_array, m + 1, r);
      merge(param_array, l, m, r);
  }
}

static double best_move(state_t *state, param_state_t *param, tetriminos_t *best_block, int index, int total) {
    double best_loss = 0;
    spawnTetriminos(state);
    /* try different rotation/direction */
    for (int i = 0; i < 4; i++) {
        tetriminos_t *active_block = state->block;
        state->rotation = clockwise(active_block, state->rotation);
        state->pos.x = 0;
        while (canMove(state)) {
            while(dropPiece(state));
            double curr_loss = 0;
            if (index == (total - 1)) {
                curr_loss = -param->aggregate_height_w*get_aggregate_height(state)
                        + param->complete_line_w*get_complete_line(state)
                        - param->hole_number_w*get_hole_number(state)
                        - param->bumpiness_w*get_bumpiness(state);
            } else {
                curr_loss = best_move(state, param, best, index + 1, total);
            }

            if (curr_loss > best_loss) {
                best_loss = curr_loss;
                state->block->num_spin = state->rotation;
                *best_tetriminos = state->block;
            }

            state->pos.x++;
        }
    }

    return best_loss;
}

void compute_loss(param_state_t *param, int iterations, int max_pieces) {
    double param_loss = 0;
    for (int i = 0; i < iterations; i++) {
        state_t state = initState();
        double loss = 0;
        double num_pieces = 0;
        while (num_pieces++ < max_pieces && canMove(state)) {
            tetriminos_t *best_block = NULL;
            best_move(state, param, best_block, 0, 2);
            while (dropPiece(state));
            /* Must separate clearLines and dropPiece */
            loss += clearLines(state->grid);
        }
        param_loss += loss;
    }
    param->loss = param_loss;
}

param_state_t **select_fittest(param_state_t **param_array, int array_size, double percentage, int num) {
    int sample_size = (int) array_size * percentage;
    param_state_t **sample = calloc(sample_size, sizeof(param_state_t *));
    for (int i = 0; i < sample_size; i++) {
        sample[i] = param_array[randomInteger(0, sample_size - 1)];
    }
    sort_param_array(sample, 0, sample_size);
    sample = realloc(sample, (num + 1) * sizeof(param_state_t *));
    sample[num] = NULL;

    return sample;
}

void generate_child(param_state_t **prev_param_array, param_state_t **param_array, int array_size) {
    param_state_t *param = malloc(sizeof(param_state_t));
    param->aggregate_height_w = 0;
    param->complete_line_w = 0;
    param->hole_number_w = 0;
    param->bumpiness_w = 0;
    for (int i = 0; prev_param_array[i] != NULL; i++) {
        param_state_t temp = prev_param_array[i];
        param->aggregate_height_w += temp->aggregate_height_w * temp->loss;
        param->complete_line_w += temp->complete_line_w * temp->loss;
        param->hole_number_w += temp->hole_number_w * temp->loss;
        param->bumpiness_w += temp->bumpiness_w * temp->loss;
    }
    normalize(param);
    if (randomInteger(0, 100) < 5) {
        if (randomInteger(0, 100) < 25)
            param->aggregate_height_w += randomDouble(-0.2, 0.2);
        else if (randomInteger(0, 100) < 25)
            param->complete_line_w += randomDouble(-0.2, 0.2);
        else if (randomInteger(0, 100) < 25)
            param->hole_number_w += randomDouble(-0.2, 0.2);
        else
            param->bumpiness_w += randomDouble(-0.2, 0.2);
    }
    normalize(param);
    param_array[array_size - 1] = param;
    sort_param_array(param_array, 0, array_size);
}