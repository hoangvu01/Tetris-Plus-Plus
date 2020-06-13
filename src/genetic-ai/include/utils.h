/* Here resides the helper functions for training the AI,
 * such as compute the aggregate height/complete lines/holes/bumpiness,
 * calculation of finess function, selecting next generation, etc.
 * GA stands for Genetic AI
 */

#ifndef GA_UTILS
#define GA_UTILS

double get_aggregate_height(grid_t grid);
double get_complete_lines(grid_t grid);
double get_holes_number(grid_t grid);
double get_bumpiness(grid_t grid);

double normalize(param_state_t param);
param_state_t generate_random_candidate();
