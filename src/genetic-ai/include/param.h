/* This file contains the state of the training after each generation 
 * GA stands for genetic AI
 */

#ifndef GA_PARAM_STATE
#define GA_PARAM_STATE

typedef struct {
  double aggregate_height_w; //weight for the aggregate height
  double complete_line_w; //weight for number of complete lines
  double hole_number_w; //weight for the number of holes
  double bumpiness_w; //weight for the numpiness
} param_state_t;

#endif
