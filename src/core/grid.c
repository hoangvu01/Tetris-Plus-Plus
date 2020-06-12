#include "grid.h"

grid_t initGrid() {
  grid_t grid = calloc(GHEIGHT, sizeof(colour_t *));

  for (int i = 0; i < GHEIGHT; i++) {
    grid[i] = calloc(GWIDTH, sizeof(colour_t));
    for (int j = 0; j < GWIDTH; j++) {
      grid[i][j] = 0;
    }
  }
  return grid;
}

void printGrid(grid_t grid) {
  /* top 2 rows are hidden */
  for (int i = 2; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      printf("%d", grid[i][j]);
    }
    printf("\n");
  }
}

colour_t *getSquare(grid_t grid, position_t p) {
  return &(grid[p.y][p.x]);
}

void freeGrid(grid_t grid) {
  for (int i = 0; i < GHEIGHT; i++) {
    free(grid[i]);
  }
  free(grid);
}