#include "utils.h"

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

static int get_column_height(grid_t grid, int c) {
    for (int y = 0; y < GHEIGHT; y++) {
        if (grid[c][y] != 0) {
            return GHEIGHT - y;
        }
    }
}

int get_aggregate_height(grid_t grid) {
    int h = 0;
    for (int x = 0; x < GWIDTH; x++) {
        h += get_column_height(grid, x);
    }

    return h;
}

int get_complete_line(grid_t grid) {
    return clearLines(grid);
}

int get_hole_number(grid_t grid) {
    bool is_block = false;
    int n = 0;
    for (int x = 0; x < GWIDTH; x++) {
        for (int y = 0; y < GHEIGHT; y++) {
            if (grid[y][x] != 0) {
                is_block = true;
            } else if (grid[y][x] == 0 && is_block) {
                n++;
            }
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

void print_and_save_result(param_state_t **array, bool is_saving) {
    param_state_t *first = array[0];
    param_state_t *second = array[1];
    printf("The first fittest param: loss - %f, aggregate_height_w - %f, complete_line_w - %f, hole_number_w - %f, bumpiness_w - %f\n",
            first->loss, first->aggregate_height_w, first->complete_line_w, first->hole_number_w, first->bumpiness_w);
    printf("The second fittest param: loss - %f, aggregate_height_w - %f, complete_line_w - %f, hole_number_w - %f, bumpiness_w - %f\n",
           second->loss, second->aggregate_height_w, second->complete_line_w, second->hole_number_w, second->bumpiness_w);
    if (is_saving) {
        FILE *fp = fopen("training_progress.txt", "w");
        /* NOT FINISHED */
        fprintf(fp, "");
        fclose(fp);
    }
}