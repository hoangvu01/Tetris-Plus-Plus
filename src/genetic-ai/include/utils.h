/* Here resides the helper functions for training the AI,
 * such as compute the aggregate height/complete lines/holes/bumpiness,
 * calculation of finess function, selecting next generation, etc.
 * GA stands for Genetic AI
 */

#ifndef GA_UTILS
#define GA_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define randomInteger(min, max)  (rand() % (max - min) + min)
#define randomDouble(min, max)  (rand() % 100 / 100.0) * (max - min) + min
/*
 * @brief: normalize the fields in parameter vector by its unit length
 * @param: param_state_t *param: the parameter to be normlized
 */
void normalize(param_state_t *param);

/*
 * @brief: generate random parameter vector at first, similar to randomize the parameters in an artificial neural network
 * @return: return the pointer to the random parameter vector
 */
param_state_t *generate_random_param();

/*
 * @brief: free the parameter vector
 */
void free_param(param_state_t *param);

/*
 * @brief: initialize an empty array of parameter vectors with the given size. The array is also NULL-terminated
 * @param: int size: the size of the array
 * @return: the pointer to the array of parameter vectors with the given size
 */
param_state_t **init_param_array(int size);

/*
 * @brief: free the array of parameter vectors
 */
void free_param_array(param_state_t **param_array);

/*
 * @brief: sort a list of parameter vectors by fitness
 * @param: param_state_t **param_array: an array of parameter vectors which will be sorted by fitness
 */
void sort_param_array(param_state_t **param_array);

/*
 * @brief: compute and set the fitness value to the given parameter vector
 * @param: param_state_t *param: the parameter vector to be set
 * @param: int iterations: specify how many simulations/rounds of games needed in order to compute the loss function for the given parameter vector
 * @param: int maxPieces: maximum number of pieces needed for each simulation/round of game
 */
void computeLoss(param_state_t *param, int iterations, int maxPieces);

/*
 * @brief: randomly select certain percent of the parameter vectors in the param_array and return only the fittest ones among those
 * @param: param_state_t **param_array: the array from which the selection will take place
 * @param: double percentage: the percentage by which the selection will be conducted on the given param_array
 * @param: int num: decide how many fittest ones to return by their loss value
 * @return: return the pointer to a array of length num which contain the fittest individual parameter vectors
 */
param_state_t **selectFittest(param_state_t **param_array, double percentage, int num);

/*
 * @brief: generate the next generation of parameter vector using cross-over and insert them into the parameter vector array
 * @param: param_state_t **prev_param_array: fittest parameter vectors from previous generation
 * @param: param_state_t **param_array: the array where the fittest individuals will be inserted, sorted by the loss value
 */
param_state_t *generateChild(param_state_t **prev_param_array, param_state_t **param_array);
