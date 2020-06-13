#include "utils.h"

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

void sort_param_array(param_state_t **param_array) {
  int i = 0;
  for (int i = 0; param_array[i] != NULL; i++) {
    for (int j = 0; param_array[j] != NULL; j++) {
      /* sort the param_array */
    }
  }
}

void computeLoss(param_state_t *param, int iterations, int maxPieces) {
  //TODO
}

param_state_t **selectFittest(param_state_t **param_array, double percentage, int num) {
  //TODO
}

param_state_t *generateChild(param_state_t **prev_param_array, param_state_t **param_array) {
  
}
