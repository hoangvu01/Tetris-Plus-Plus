/* This file contains the state of the training after each generation 
 * GA stands for genetic AI
 */

#ifndef GA_PARAM_STATE
#define GA_PARAM_STATE

typedef struct {
  double height_w; //weight for the aggregate height
  double line_w; //weight for number of complete lines
  double hole_w; //weight for the number of holes
  double bumpiness_w; //weight for the numpiness
} param_state_t;

double get_height_weight();
double get_line_weight();
double get_hole_weight();
double get_bumpiness_weight();

void set_height_weight(double value);
void set_line_weight(double value);
void set_hole_weight(double value);
void set_bumpiness_weight(double value);

#endif
