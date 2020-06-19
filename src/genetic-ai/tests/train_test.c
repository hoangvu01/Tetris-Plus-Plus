#include "train.h"
#include "testutils.h"

int main(void) {
  /* Testing generate_random_param()... */
  char name[] = "testing whether a random parameter has generated...";
  param_state_t *param = generate_random_param();
  fprintf(stdout, "please manually check the randomness of the following numbers:\n");
  fprintf(stdout, "aggregate_height_w: %lf\n", param->aggregate_height_w);
  fprintf(stdout, "complete_line_w: %lf\n", param->complete_line_w);
  fprintf(stdout, "hole_number_w: %lf\n", param->hole_number_w);
  fprintf(stdout, "bumpiness_w: %lf\n", param->bumpincess_w);
  fprintf(stdout, "loss: %d\n", param->loss);
}
