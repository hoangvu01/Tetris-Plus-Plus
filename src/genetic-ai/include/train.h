/*
 * train.h contains functions related to genetic algorithm:
 *
 */
#ifndef TETRIS_PLUS_PLUS_TRAIN_H
#define TETRIS_PLUS_PLUS_TRAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "utils.h"

#define randomInteger(min, max)  (rand() % (max - min) + min)
#define randomDouble(min, max)  (rand() % 100 / 100.0) * (max - min) + min

/*
 * the entry point(main logic) of training
 */
void train(char *filename);

/*
 * @brief: generate random parameter vector at first, similar to randomize the parameters in an artificial neural network
 * @return: return the pointer to the random parameter vector
 */
param_state_t *generate_random_param();

/*
 * @brief: initialize an empty array of parameter vectors with the given size. The array is also NULL-terminated
 * @param: int size: the size of the array
 * @return: the pointer to the array of parameter vectors with the given size
 */
param_state_t **init_param_array(int size);

/*
 * @brief: helper function for best_move. Used in play.c as well
 */

double immutable_best_move(const state_t *state, const param_state_t *param, block_t *best_block, int total_lines_cleared, bool is_conservative);

/*
 * @brief: calculate the best move from the given block and grid layout with the given parameter vector
 * @param: state_t *state: the current state of the game (see state.h in core folder)
 * @param: param_state_t *param: the given parameter vector used in calculating loss
 * @param: tetriminos_t *best_block: the pointer to the block that will be set to the best move
 * @param: int index and int total: total is used to track how many blocks to look ahead and index is used for termination of recursion
 */
int best_move(state_t *state, const param_state_t *param);


/*
 * @brief: compute and set the fitness value to the given parameter vector
 * @param: param_state_t *param: the parameter vector to be set
 * @param: int iterations: specify how many simulations/rounds of games needed in order to compute the loss function for the given parameter vector
 * @param: int maxPieces: maximum number of pieces needed for each simulation/round of game
 */
void compute_loss(param_state_t *param, int iterations, int max_pieces);

/*
 * @brief: randomly select certain percent of the parameter vectors in the param_array and return only the fittest ones among those
 * @param: param_state_t **param_array: the array from which the selection will take place
 * @param: double percentage: the percentage by which the selection will be conducted on the given param_array
 * @param: int num: decide how many fittest ones to return by their loss value
 * @return: return the pointer to a array of length num which contain the fittest individual parameter vectors
 */
param_state_t **select_fittest(param_state_t **param_array, int array_size, double percentage, int num);

/*
 * @brief: generate the next generation of parameter vector using cross-over and insert them into the parameter vector array
 * @param: param_state_t **fittest: fittest parameter vectors from previous generation
 * @param: param_state_t **param_array: the offspring array where the fittest individuals will be inserted, sorted by the loss value
 */
bool generate_child(param_state_t **fittest, param_state_t **param_array, int array_size);


#endif
