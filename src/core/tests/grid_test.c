#include "grid.h"

#include "testutils.h"

int main() {

  /* init grid is all zero*/
  grid_t grid = initGrid();
  for (int i = 0; i < GHEIGHT; i++) {
    for (int j = 0; j < GWIDTH; j++) {
      testint(grid[i][j], 0, "Init grid is all zero. ");
    }
  }
	printf("\n\n");

  
	/* clone grid is the same as being cloned*/
  grid[5][6] = 15;
  grid[3][15] = 24;
	testint(grid[5][6], 15, "Clone grid is the same as being cloned. ");
	testint(grid[3][15], 24, "Clone grid is the same as being cloned. ");
	testint(grid[7][14], 0, "Clone grid is the same as being cloned. ");
	printf("\n\n");

	/*get square returns correctly. */
	position_t pos1 = {6,5};
	position_t pos2 = {15,3};
	position_t pos3 = {14,7};

	testint(*(getSquare(grid, pos1)), grid[5][6], "Get square returns correctly.");
	testint(*(getSquare(grid, pos2)), grid[3][15], "Get square returns correctly.");
	testint(*(getSquare(grid, pos3)), grid[7][14], "Get square returns correctly.");
	
	printf("\n\n");

  return EXIT_SUCCESS;
}
