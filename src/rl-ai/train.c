#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "qlearn.h"


void train() {
  q_data_t *data = malloc(sizeof(q_data_t));
  play(data);
}

int main() {
  train(); 
  return EXIT_SUCCESS;
}
