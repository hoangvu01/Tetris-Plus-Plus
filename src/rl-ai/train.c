#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "qlearn.h"
#include "qtable.h"


#define ALPHA 0.1
#define GAMMA 0.5
#define EPSILON 0.1


void load_qtable(q_data_t *data){
  FILE *fp;
  fp = fopen("qtable.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file");
    return;
  }

  char *line = NULL;
  char **str = malloc(sizeof(char *));
  size_t len = 0;
  long key;
  double *actions = calloc(6, sizeof(double));
  while (getline(&line, &len, fp) != -1) {
    key = strtol(line, str, 10);
    strtok(line, "[");
    *str = strtok(NULL, "[");
    strtok(*str, " ");
    for (int i = 0; i < 6; i++) {
      sscanf(strtok(NULL, " "), "%lf", actions + i);
    }
    load_hash_qtable(data->qtable, &key, actions);
  }

  fclose(fp);
  if (line) free(line);
  free(str);
}

void write_qtable(q_data_t *data, int episode) {
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
  q_data_t *data = calloc(1, sizeof(q_data_t));
  data->qtable = init_qtable();
  load_qtable(data);
  for (int i = 1; i <= episodes; i++) {
    int score = play(data);
    endwin();
    wprintw(stdscr, "Score : %d\n", score);  
    wprintw(stdscr, "====Episode %d====\n", i);
    fprintf(stdout, "Score : %d\n", score);
    fprintf(stdout, "====Episode %d====\n", i);     
    
    if (i % 10 == 0) write_qtable(data, i); 
  }
}

int main() {
  train(10); 
  return EXIT_SUCCESS;
}
