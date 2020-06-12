#include "grid.h"

square **initGrid() {
  square **grid = calloc(GHEIGHT, sizeof(square *));

  for (int i = 0; i < GHEIGHT; i++) {
    grid[i] = calloc(GWIDTH, sizeof(square));
    for (int j = 0; j < GWIDTH; j++) {
      grid[i][j].colour = 0;
      grid[i][j].isStatic = true; 
    }
  }
	return grid;
}

void freeGrid(square **grid) {
  for (int i = 0; i < GHEIGHT; i++) {
    free(grid[i]);
  }
	free(grid);
}