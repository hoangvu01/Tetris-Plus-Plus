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
#include <assert.h>
#include "param.h"
#include "grid.h"
#include "state.h"
#include "tetriminos.h"

typedef struct {
    int id;
    position_t pos;
    int rotation;
} block_t;

double cap_to_nonnegative(double n);

block_t* init_block_from_state(const state_t *state);
block_t* clone_block(const block_t *block);
void free_block(block_t *block);
void set_state_by_block(state_t *state, const block_t *block);
void print_block(const block_t *block);

/*
 * @brief: normalize the fields in parameter vector by its unit length
 * @param: param_state_t *param: the parameter to be normlized
 */
void normalize(param_state_t *param);

void print_param(const param_state_t *param);

/*
 * @brief: free the parameter vector/parameter vector array
 */
void free_param(param_state_t *param);
void free_param_array(param_state_t **param_array);


/*
 * @brief: drop the piece on the grid and selectively clear lines and paint grid
 */
bool dropPieceWithOptions(state_t *curr, bool clear_lines, bool color_grid, int* lines_cleared);

/*
 * @brief: sort a list of parameter vectors by fitness
 * @param: param_state_t **param_array: an array of parameter vectors which will be sorted by fitness
 * @param: int array_size: size of the param_array
 */
void sort_param_array(param_state_t **param_array, int array_size);

void print_param_array(param_state_t **param_array, int array_size);

/*
 * @brief: calculate the aggregate height/complete lines/number of holes/bumpiness of the current grid
 * @param: grid_t grid: the grid of the tetris game
 * @return: the aggregate height of the current grid layout
 */
int get_aggregate_height(grid_t grid);
int get_complete_line(grid_t grid);
int get_hole_number(grid_t grid);
int get_bumpiness(grid_t grid);
int get_risk(grid_t grid);

/*
 * @brief: printing and saving functions during training. It will save all the parameter vectors
 * in the current array and print the first few ones(the fittest ones) out in command line
 * @param: param_state_t **array: the current set of parameter vectors order by their loss
 * @param: bool is_saving: if set then it will save to a new file, if not set it will only print the fittest ones
 */
void print_and_save_result(param_state_t **array, bool is_saving);

/*
 * @brief: reading parameters from a file in order to continue training
 * @param: char *filename: the filename of the file to be read
 * @param: int array_size: the size of the parameter vector array
 * @return: an array of parameter vector that represents the data in the given file
 */
param_state_t **read_param_from_file(char *filename, int array_size);

#endif
