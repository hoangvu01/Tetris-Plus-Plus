#ifndef GRID_H
#define GRID_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetriminos.h"

typedef int colour_t;
typedef colour_t **grid_t;

#define GWIDTH 10
#define GHEIGHT 22  // Top 2 rows are hidden

/*
 * Allocated a new grid of colours for the game.
 * @returns: The grid itself as a 2D array.
 */
grid_t initGrid();

/*
 * Creates a deep clone of a grid, used to separate output grid from the grid
 * used for logic.
 * @param: The grid to clone.
 * @returns: The cloned grid as a 2D array.
 */
grid_t cloneGrid(grid_t grid);

/*
 * Gets the colour of a particular square inside the grid. 
 * used for logic.
 * @param p: The position of the square of interest, a struct of x, y coordinates. 
 * @returns: The colour of the square as an integer between 0-7.
 */
colour_t *getSquare(grid_t grid, position_t p);

int clearLines(grid_t grid);

/*
 * Deallocated the provided grid. 
 * @param: The grid to free.
 */
void freeGrid(grid_t grid);

#endif

