#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "qlearn.h"
#include "qtable.h"


#define ALPHA 0.1
#define GAMMA 0.5
#define EPSILON 0.1


void write_qtable(q_data_t *data) {
  FILE *fp;
  fp = fopen("qtable.txt", "w+");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file");
    exit(EXIT_FAILURE);
  }
  
  hash_table_itr *itr = get_hash_table_itr(data->qtable);
  
  while (hash_iterator_hasnext(itr)) {
    long key;
    double *actions = (double *) hash_iterator_next(itr, &key);
    fprintf(fp, "%ld : [", key);
    for (int i = 0; i < 6; i++) {
      fprintf(fp, "%f ", actions[i]);
    }
    fprintf(fp, "]\n");
  }
  
  fclose(fp);
  
}


void train(int episodes) {
  q_data_t *data = malloc(sizeof(q_data_t));
  data->qtable = init_qtable();
  for (int i = 1; i <= episodes; i++) {
    int score = play(data);
    endwin();
    fprintf(stdout, "Score : %d\n", score);
    fprintf(stdout, "====Episode %d====\n", i);     
    
    if (i % 100 == 0) write_qtable(data); 
  }
}

int main() {
  train(1000000); 
  return EXIT_SUCCESS;
}
