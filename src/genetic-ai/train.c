#include "train.h"
#include "utils.h"
#include <math.h>
#include <assert.h>

#define ARRAY_SIZE 100
#define OFFSPRING_SIZE 30

const int n_iterations = 5;
const int n_pieces = 200;

void train(char *filename) {
    param_state_t **param_array = NULL;
    if (filename != NULL) {
      param_array = read_param_from_file(filename, ARRAY_SIZE);
    } else {
      param_array = init_param_array(ARRAY_SIZE);
    }
    param_state_t **offspring_array = calloc(OFFSPRING_SIZE, sizeof(param_state_t *));
    printf("Computing the loss of initial population...\n");
    for (int i = 0; param_array[i] != NULL; i++) {
        compute_loss(param_array[i], n_iterations, n_pieces);
        printf("Loss successfully computed for param vec %d\n", i);
    }
    sort_param_array(param_array, ARRAY_SIZE);
    print_param_array(param_array, 5);
    print_and_save_result(param_array, 1);

    int count = 0;
    while(count < 1000) {
        for (int i = 0; i < 30; i++) {
            param_state_t **fittest = select_fittest(param_array, ARRAY_SIZE, 0.3, 2);
            if (generate_child(fittest, offspring_array, OFFSPRING_SIZE)) {
              /* delete the worst 30% of the population and add offsprings */
              for (int i = ARRAY_SIZE - OFFSPRING_SIZE; i < ARRAY_SIZE; i++) {
                param_array[i] = offspring_array[i - ARRAY_SIZE + OFFSPRING_SIZE];
              }
              //sort_param_array(param_array);
              memset(offspring_array, 0, OFFSPRING_SIZE * sizeof(param_state_t *));
            }
        }
        printf("Computing loss of new parameter vectors...\n");
        for (int i = 0; param_array[i] != NULL; i++) {
            compute_loss(param_array[i], n_iterations, n_pieces);
        }
        sort_param_array(param_array, ARRAY_SIZE);

        double total_loss = 0;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            total_loss += param_array[i]->loss;
        }
        printf("Average Loss= %f\n", total_loss);
        print_and_save_result(param_array, 1);
        count++;
    }
}

param_state_t *generate_random_param() {
    param_state_t *param = malloc(sizeof(param_state_t));
    if (param == NULL) {
        perror("generate_random_param: Failed to generate random parameters");
        exit(EXIT_FAILURE);
    }
    param->aggregate_height_w = randomDouble(0, 1);
    param->complete_line_w = randomDouble(0, 1);
    param->hole_number_w = randomDouble(0, 1);
    param->bumpiness_w = randomDouble(0, 1);
    /* risk parameter is currently disabled */
    //param->risk_w = randomDouble(0, 1);
    param->loss = INT_MAX;

    return param;
}

param_state_t **init_param_array(int size) {
    param_state_t **array = calloc(size + 1, sizeof(param_state_t *));
    for (int i = 0; i < size; i++) {
        array[i] = generate_random_param();
    }
    array[size] = NULL;

    return array;
}

// find out the best move without altering the state
double immutable_best_move(const state_t *state, const param_state_t *param, block_t *best_block, int total_lines_cleared, bool is_conservative) {
    double best_loss = -INFINITY;

    /* try different rotation/direction */
    for (int i = 0; i < state->block->num_spin; i++) {
        for (int j = 0; j < GWIDTH; j++) {
            state_t *new_state = cloneState(state);
            new_state->rotation = i;
            new_state->pos.x = j;
            if (canMove(new_state)) {
                int lines_cleared = 0;
                while (dropPieceWithOptions(new_state, true, true, &lines_cleared));
		int score[5] = {0, 40, 100, 300, 1200};
                total_lines_cleared += is_conservative ? lines_cleared : score[lines_cleared];

                double curr_loss = 0.0;
                block_t *old_block = NULL;
                if (state->nextBlock == NULL) {
                    curr_loss = - param->aggregate_height_w * get_aggregate_height(new_state->grid)
                                - param->hole_number_w * get_hole_number(new_state->grid)
                                - param->bumpiness_w * get_bumpiness(new_state->grid)
                                + param->complete_line_w * total_lines_cleared;
                                //+ param->risk_w * get_risk(new_state->grid);
                } else {
                    old_block = init_block_from_state(new_state);
                    //print_block(old_block);
                    new_state->block = new_state->nextBlock;
                    new_state->pos.y = 2;
                    new_state->nextBlock = NULL;
                    curr_loss = immutable_best_move(new_state, param, NULL, total_lines_cleared, is_conservative);
                }

                if (curr_loss > best_loss) {
                    best_loss = curr_loss;
                    if (best_block != NULL) *best_block = *old_block;
                }

                if (old_block != NULL) free_block(old_block);
            }

            freeState(new_state);
        }
    }
    return best_loss;
}

// make the best move, and return the number of lines cleared in this move, or -1 if no possible move
int best_move(state_t *state, const param_state_t *param) {
    block_t best_block;
    best_block.id = -1;
    immutable_best_move(state, param, &best_block, 0, false);
    if (best_block.id == -1) return -1; // no possible move
    //print_block(&best_block);
    set_state_by_block(state, &best_block);
    int lines_cleared;
    while(dropPieceWithOptions(state, true, true, &lines_cleared));
    return lines_cleared;
}

void compute_loss(param_state_t *param, int iterations, int max_pieces) {
    int param_loss = 0;

    #pragma omp parallel for reduction(+: param_loss)
    for (int i = 0; i < iterations; i++) {
        state_t *state = initState();
        int loss = 0;
        double num_pieces = 0;
        while (num_pieces++ < max_pieces && spawnTetriminos(state)) {
            int curr_loss = best_move(state, param);
            if (curr_loss == -1) {
                printf("no more moves available, skipping\n");
                break;
            }
            loss += curr_loss * curr_loss;
        }
        param_loss += loss;
        freeState(state);
    }
    printf("param_loss = %d\n", param_loss);
    param->loss = param_loss;
}

param_state_t **select_fittest(param_state_t **param_array, int array_size, double percentage, int num) {
    int sample_size = (int) array_size * percentage;
    param_state_t **sample = calloc(sample_size, sizeof(param_state_t *));
    bool *used = calloc(array_size, sizeof(bool));
    for (int i = 0; i < sample_size; i++) {
        int random = -1;
        while (random == -1 || used[random]) random = randomInteger(0, array_size);
        used[random] = true;
        sample[i] = param_array[random];
    }
    sort_param_array(sample, sample_size);
    sample = realloc(sample, (num + 1) * sizeof(param_state_t *));
    sample[num] = NULL;

    free(used);
    return sample;
}

bool generate_child(param_state_t **fittest, param_state_t **param_array, int array_size) {
    param_state_t *param = malloc(sizeof(param_state_t));
    param->aggregate_height_w = 0;
    param->complete_line_w = 0;
    param->hole_number_w = 0;
    param->bumpiness_w = 0;
    for (int i = 0; fittest[i] != NULL; i++) {
        param_state_t *temp = fittest[i];
        param->aggregate_height_w += temp->aggregate_height_w * temp->loss;
        param->complete_line_w += temp->complete_line_w * temp->loss;
        param->hole_number_w += temp->hole_number_w * temp->loss;
        param->bumpiness_w += temp->bumpiness_w * temp->loss;
        /* risk is currently disabled */
        //param->risk_w += temp->risk_w * temp->loss;
    }
    normalize(param);

    // mutate
    if (randomInteger(0, 100) < 5) {
        int choice = randomInteger(0, 4);
        switch (choice) {
        case 0:
            param->aggregate_height_w = cap_to_nonnegative(param->aggregate_height_w + randomDouble(-0.2, 0.2));
            break;
        case 1:
            param->complete_line_w = cap_to_nonnegative(param->complete_line_w + randomDouble(-0.2, 0.2));
            break;
        case 2:
            param->hole_number_w = cap_to_nonnegative(param->hole_number_w + randomDouble(-0.2, 0.2));
            break;
        case 3:
            param->bumpiness_w = cap_to_nonnegative(param->bumpiness_w + randomDouble(-0.2, 0.2));
            break;
        }
    }
    normalize(param);

    int i = 0;
    while (param_array[i] != NULL) {
      i++;
    }
    if (i >= array_size) {
      return true;
    } else {
      param_array[i] = param;
      return false;
    }
}
