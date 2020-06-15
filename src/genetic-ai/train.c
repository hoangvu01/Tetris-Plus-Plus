#include "train.h"
#include "utils.h"
#include <assert.h>

#define ARRAY_SIZE 100

int main(void) {
    param_state_t **param_array = init_param_array(ARRAY_SIZE);
    printf("Computing the loss of initial population...\n");
    for (int i = 0; param_array[i] != NULL; i++) {
        compute_loss(param_array[i], 5, 200);
        printf("Loss successfully computed for param vec %d\n", i);
    }
    sort_param_array(param_array, 0, ARRAY_SIZE - 1);
    print_and_save_result(param_array, 1);

    int count = 0;
    while(count < 1000) {
        for (int i = 0; i < 30; i++) {
            param_state_t **fittest = select_fittest(param_array, ARRAY_SIZE, 0.3, 2);
            generate_child(fittest, param_array, ARRAY_SIZE);
        }
        printf("Computing loss of new parameter vectors...\n");
        for (int i = 0; param_array[i] != NULL; i++) {
            compute_loss(param_array[i], 5, 200);
        }
        sort_param_array(param_array, 0, ARRAY_SIZE - 1);

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
    param->loss = (double) INT_MAX;

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

double best_move(state_t *state, param_state_t *param, tetrimino_t *best_block, int index, int total) {
    double best_loss = 0;
    /* try different rotation/direction */
    for (int i = 0; i < 4; i++) {
        tetrimino_t *active_block = state->block;
        state->rotation = clockwise(active_block, state->rotation);
        state->pos.x = 0;
        while (canMove(state)) {
            while(dropPiece(state));
            double curr_loss = 0;
            if (index == (total - 1)) {
                curr_loss = - param->aggregate_height_w*get_aggregate_height(state->grid)
                            - param->hole_number_w*get_hole_number(state->grid)
                            - param->bumpiness_w*get_bumpiness(state->grid)
                            + param->complete_line_w*get_complete_line(state->grid);
            } else {
                state_t *cloned_state = cloneState(state);
                spawnTetriminos(cloned_state);
                curr_loss = best_move(cloned_state, param, best_block, index + 1, total);
            }

            if (curr_loss > best_loss) {
                best_loss = curr_loss;
                best_block = state->block;
            }

            state->pos.x++;
        }
    }

    return best_loss;
}

void compute_loss(param_state_t *param, int iterations, int max_pieces) {
    double param_loss = 0;
    for (int i = 0; i < iterations; i++) {
        state_t *state = initState();
        double loss = 0;
        double num_pieces = 0;
        while (num_pieces++ < max_pieces && spawnTetriminos(state)) {
            tetrimino_t *best_block = NULL;
            best_move(state, param, best_block, 0, 2);
            while (dropPiece(state));
            /* Must separate clearLines and dropPiece */
            loss += clearLines(state->grid);
        }
        param_loss += loss;
        //freeState(state);
    }
    param->loss = param_loss;
}

param_state_t **select_fittest(param_state_t **param_array, int array_size, double percentage, int num) {
    int sample_size = (int) array_size * percentage;
    param_state_t **sample = calloc(sample_size, sizeof(param_state_t *));
    for (int i = 0; i < sample_size; i++) {
        sample[i] = param_array[randomInteger(0, sample_size - 1)];
    }
    sort_param_array(sample, 0, sample_size - 1);
    sample = realloc(sample, (num + 1) * sizeof(param_state_t *));
    sample[num] = NULL;

    return sample;
}

void generate_child(param_state_t **prev_param_array, param_state_t **param_array, int array_size) {
    param_state_t *param = malloc(sizeof(param_state_t));
    param->aggregate_height_w = 0;
    param->complete_line_w = 0;
    param->hole_number_w = 0;
    param->bumpiness_w = 0;
    for (int i = 0; prev_param_array[i] != NULL; i++) {
        param_state_t *temp = prev_param_array[i];
        param->aggregate_height_w += temp->aggregate_height_w * temp->loss;
        param->complete_line_w += temp->complete_line_w * temp->loss;
        param->hole_number_w += temp->hole_number_w * temp->loss;
        param->bumpiness_w += temp->bumpiness_w * temp->loss;
    }
    normalize(param);
    if (randomInteger(0, 100) < 5) {
        if (randomInteger(0, 100) < 25)
            param->aggregate_height_w += randomDouble(-0.2, 0.2);
        else if (randomInteger(0, 100) < 25)
            param->complete_line_w += randomDouble(-0.2, 0.2);
        else if (randomInteger(0, 100) < 25)
            param->hole_number_w += randomDouble(-0.2, 0.2);
        else
            param->bumpiness_w += randomDouble(-0.2, 0.2);
    }
    normalize(param);
    param_array[array_size - 1] = param;
    sort_param_array(param_array, 0, array_size - 1);
}
