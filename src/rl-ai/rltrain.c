#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ncurses.h>

#include "qlearn.h"
#include "qtable.h"
#include "env_defns.h"


void load_qtable(q_data *data){
  FILE *fp;
  fp = fopen("qtable_result.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file");
    return;
  }

  char *line = NULL;
  size_t len = 0;
  env_t env;
  while (getline(&line, &len, fp) != -1) {
    int *elevation = calloc(GWIDTH, sizeof(int));
    double *actions = calloc(6, sizeof(double));
    env.elevation = elevation;
    
    char *elev, *block, *act;
    elev = strtok(line, ",");
    block = strtok(NULL, ",");
    act = strtok(NULL, ",");
    
    if (elev[1] == 'p') break;

    /* Scan elevation */
    elev = strtok(elev, "[");
    for (int i = 0; i < GWIDTH; i++) {
      elev = strtok(NULL, " ");
      sscanf(elev, "%d",  elevation + i);
    }
    
    /* Scan block */
    block = strtok(block, "[");
    sscanf(strtok(NULL, " "), "%d", &env.block);
    sscanf(strtok(NULL, " "), "%d", &env.spin);
    sscanf(strtok(NULL, " "), "%d", &env.block_x); 

    /* Scan actions */
    act = strtok(act, "[");
    for (int i = 0; i < 6; i++) {
      act = strtok(NULL, " ");
      sscanf(act, "%lf", actions + i);
    }

    insert_qtable(data->qtable, &env, actions);
  }

  fclose(fp);
  if (line) free(line);
}

void write_qtable(q_data *data, int episode) {
  FILE *fp;
  fp = fopen("qtable_result.txt", "w+");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file");
    exit(EXIT_FAILURE);
  }
  
  hash_table_itr *itr = get_hash_table_itr(data->qtable);
  
  while (hash_iterator_hasnext(itr)) {
    env_t env;
    double *actions = (double *) hash_iterator_next(itr, &env);
    fprintf(fp, "Elev : [" );
    for (int i = 0; i < GWIDTH; i++) {
      fprintf(fp, "%d ", env.elevation[i]);
    }
    fprintf(fp, "], Block: [%d %d %d], ", env.block, env.spin, env.block_x);
    fprintf(fp, "Actions : [");
    for (int i = 0; i < 6; i++) {
      fprintf(fp, "%f ", actions[i]);
    }
    fprintf(fp, "]\n");
  }
  
  fprintf(fp, "Episode: %d", episode);
  fclose(fp);
  
}


void train(int episodes) {
  q_data *data = calloc(1, sizeof(q_data));
  data->qtable = init_qtable();
  load_qtable(data);
  for (int i = 1; i <= episodes; i++) {
    int score = play(data);
    endwin();
    wprintw(stdscr, "You scored: %d @ Episode %d\n", score, i);

    if (i % 50 == 0) write_qtable(data, i); 
  }
  free_qtable(data->qtable);
  free(data);
}

int main() {
  train(1); 
  return EXIT_SUCCESS;
}
