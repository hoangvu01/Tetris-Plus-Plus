#include "utils.h"
#include "testutils.h"

/* train.h is included because some of the function needed reside here */
#include "train.h"

int main(void) {
  /* Testing normalize()... */
  char name[] = "Testing normalize() in utils.c";
  param_state_t *param = malloc(param_state_t);
  param->aggregate_height_w = 1.2;
  param->complete_line_w = 1.5;
  param->hole_number_w = 1.5;
  param->bumpiness_w = 1.2;
  normalize(param);
  testdouble(1.2/2.7166155414, param->aggregate_height_w, name);
  testdouble(1.5/2.7166155414, param->complete_line_w, name);
  testdouble(1.5/2.7166155414, param->hole_number_w, name);
  testdouble(1.2/2.7166155414, param->bumpiness_w, name);
  free_param(param);

  /* Testing sort_param_array() ... */
  name = "Testing if the array can be sorted properly in utils.c";
  /* init_param_array() is assumed to be working here since it can be tested in train_test.c as well */
  param_state_t param_array = init_param_array(10);
  sort_param_array(param_array, 10);
  int loss = param_array[0]->loss;
  for (int i = 1; i < 10; i++) {
    assert (loss >= param_array[i]->loss);
    loss = param_array[i]->loss;
  }

  /* Testing get_aggregate_height()... */
  name = "Testing whether get_aggregate_height() can get total height properly in utils.c";
  grid_t grid = initGrid();
  for (int i = 0; i < GWIDTH; i++) {
    grid[19][i] = 1;
  }
  testint(30, get_aggregate_height(grid), name);
  freeGrid(grid);

  /* Testing get_complete_line()... */
  name = "Testing if get_complete_line() works properly in utils.c";
  grid_t grid = initGrid();
  for (int i = 0; i < GWIDTH; i++) {
    grid[19][i] = 1;
    grid[20][i] = 1;
    grid[21][i] = 2;
  }
  testint(3, get_complete_line(grid), name);

  /* Testing get_hole_number()... */
  name = "Testing if get_hole_number() works properly in utils.c";
  grid[19][3] = 0;
  grid[20][5] = 0;
  testint(2, get_hole_number(grid), name);

  /* Testing get_bumpiness()... */
  name = "Testing if get_bumpiness() works properly in utils.c";
  testint(2, get_bumpiness(grid), name);
  freeGrid(grid);

  /*
   * To test the rest of the functions, you need to do it manually. (e.g. run `./genetic-train <filename>` to see whether the param_array has been loaded from the given file)
   */
}
