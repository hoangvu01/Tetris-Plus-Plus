#include "state.h"
#include "testutils.h"

int main(){
	/* tetriminos sets num_spin correctly*/
	tetrimino_t *list = initTetrimino();
	testint(list[TBLOCK].num_spin, 4, "Correct number of spins. ");
	testint(list[JBLOCK].num_spin, 4, "Correct number of spins. ");
	testint(list[ZBLOCK].num_spin, 2, "Correct number of spins. ");
	testint(list[OBLOCK].num_spin, 1, "Correct number of spins. ");
	testint(list[SBLOCK].num_spin, 2, "Correct number of spins. ");
	testint(list[LBLOCK].num_spin, 4, "Correct number of spins. ");
	testint(list[IBLOCK].num_spin, 2, "Correct number of spins. ");

	/* tetriminos sets num_spin correctly*/
	testint(list[TBLOCK].spins[1][2].x,  0, "Tetriminos sets positions correctly. ");
	testint(list[JBLOCK].spins[1][0].x,  0, "Tetriminos sets positions correctly. ");
	testint(list[ZBLOCK].spins[0][3].y,  1, "Tetriminos sets positions correctly. ");
	testint(list[OBLOCK].spins[0][2].y,  1, "Tetriminos sets positions correctly. ");
	testint(list[SBLOCK].spins[0][2].x, -1, "Tetriminos sets positions correctly. ");
	testint(list[LBLOCK].spins[0][0].x, -1, "Tetriminos sets positions correctly. ");
	testint(list[IBLOCK].spins[0][0].x, -2, "Tetriminos sets positions correctly. ");


	/* rotation is correct */
	testint(clockwise(list + TBLOCK, 0), 1, "Rotation is correct. ");
	testint(clockwise(list + OBLOCK, 0), 0, "Rotation is correct. ");
	testint(clockwise(list + SBLOCK, 1), 0, "Rotation is correct. ");
	testint(antiClockwise(list + TBLOCK, 0), 3, "Rotation is correct. ");
	testint(antiClockwise(list + OBLOCK, 0), 0, "Rotation is correct. ");
	testint(antiClockwise(list + SBLOCK, 1), 0, "Rotation is correct. ");

	/* position combination is correct */
	position_t dst;
	position_t base = {0, 0}, shift = {0, 0};
	pplus(&dst, base, shift);
	testint(dst.x, 0, "position combination is correct. ");
	testint(dst.y, 0, "position combination is correct. ");

	position_t base1 = {-1, 1}, shift1 = {2, 5};
	pplus(&dst, base1, shift1);
	testint(dst.x, 1, "position combination is correct. ");
	testint(dst.y, 6, "position combination is correct. ");

	position_t base2 = {514, -234}, shift2 = {-1351, 51231};
	pplus(&dst, base2, shift2);
	testint(dst.x, 514 - 1351, "position combination is correct. ");
	testint(dst.y, -234 + 51231, "position combination is correct. ");
}