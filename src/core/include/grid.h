#ifndef GRID_H
#define GRID_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int colour;
	bool isStatic;
} square;

#define GWIDTH 10
#define GHEIGHT 20

square **initGrid();

void freeGrid(square **grid);

#endif