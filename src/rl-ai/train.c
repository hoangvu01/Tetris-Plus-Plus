#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "qlearn.h"
#include "qtable.h"


#define ALPHA 0.1
#define GAMMA 0.5
#define EPSILON 0.1



void train(int episodes) {
  for (int i = 1; i <= episodes; i++) {
    q_data_t *data = malloc(sizeof(q_data_t));
    data->qtable = init_qtable();
    play(data);
    
    
  }
}

int main() {
  train(1); 
  return EXIT_SUCCESS;
}
