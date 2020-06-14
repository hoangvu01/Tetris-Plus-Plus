/* Here resides the helper functions for training the AI,
 * such as compute the aggregate height/complete lines/holes/bumpiness,
 * calculation of finess function, selecting next generation, etc.
 * GA stands for Genetic AI
 */

#ifndef GA_UTILS
#define GA_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "state.h"
#include "param.h"

/*
 * @brief: normalize the fields in parameter vector by its unit length
 * @param: param_state_t *param: the parameter to be normlized
 */
void normalize(param_state_t *param);

/*
 * @brief: free the parameter vector/parameter vector array
 */
void free_param(param_state_t *param);
void free_param_array(param_state_t **param_array);

/*
 * @brief: sort a list of parameter vectors by fitness
 * @param: param_state_t **param_array: an array of parameter vectors which will be sorted by fitness
 * @param: int l: the left boundary for merge sort
 * @param: int r: the right boundary for merge sort
 */
void sort_param_array(param_state_t **param_array, int l, int r);

/*
 * @brief: calculate the aggregate height/complete lines/number of holes/bumpiness of the current grid
 * @param: grid_t grid: the grid of the tetris game
 * @return: the aggregate height of the current grid layout
 */
int get_aggregate_height(grid_t grid);
int get_complete_line(grid_t grid);
int get_hole_number(grid_t grid);
int get_bumpiness(grid_t grid);

/*
 * @brief: printing and saving functions during training. It will save all the parameter vectors
 * in the current array and print the first few ones(the fittest ones) out in command line
 * @param: param_state_t **array: the current set of parameter vectors order by their loss
 * @param: bool is_saving: if set then it will save to a new file, if not set it will only print the fittest ones
 */
void print_and_save_result(param_state_t **array, bool is_saving);