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

grid_t cloneGrid(grid_t grid) {
  grid_t clone = calloc(GHEIGHT, sizeof(colour_t *));

  for (int i = 0; i < GHEIGHT; i++) {
    clone[i] = calloc(GWIDTH, sizeof(colour_t));
    for (int j = 0; j < GWIDTH; j++) {
      clone[i][j] = grid[i][j];
    }
  }
  return clone;
}

void printGrid(grid_t grid) {
  /* top 2 rows are hidden */
  for (int i = 2; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] == 0)
        printw("  ");
      else
        printw("%d ", grid[i][j]);
    }
    printw("\n");
  }
  for (int j = 0; j < GWIDTH; j++) {
    printw("_ ");
  }
}

colour_t *getSquare(grid_t grid, position_t p) { return &(grid[p.y][p.x]); }

void freeGrid(grid_t grid) {
  for (int i = 0; i < GHEIGHT; i++) {
    free(grid[i]);
  }
  free(grid);
}

int clearLines(grid_t grid) {
  int completedlines = 0;
  /* clears top buffers */
  for (int i = 0; i < 2; i++) {
    memset(grid[i], 0x00, GWIDTH * sizeof(colour_t));
  }

  for (int i = 2; i < GHEIGHT; i++) {
    bool isComplete = true;
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] == 0) isComplete = false;
    }

    if (isComplete) {
      completedlines++;
      for (int k = i; k >= 2; k--) {
        memcpy(grid[k], grid[k - 1], GWIDTH * sizeof(colour_t));
      }
    }
  }
  return completedlines;
}