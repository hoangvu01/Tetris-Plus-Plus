/*
 * Tetriminos are stored as an array of orientations. Each orientation contains
 * the position of the 4 squares that forms a tetrimino; stored as the
 * displacement from the center of a 5x5 square. The center of this square is
 * the axis of rotation.
 */

#ifndef TMS_H
#define TMS_H

#define TBLOCK 0
#define JBLOCK 1
#define ZBLOCK 2
#define OBLOCK 3
#define SBLOCK 4
#define LBLOCK 5
#define IBLOCK 6
#define NUM_BLOCKS 8

typedef struct {
  int x;
  int y;
} position_t;

typedef position_t orientation_t[4];

typedef struct {
  int num_spin;
  orientation_t *spins;
} tetrimino_t;

/*
 * Creates a list of available tetriminos that over programs can index into. The
 * list should contain 7 elements in the standard implementation
 */
tetrimino_t *initTetrimino();

/*
 * Frees the list of tetriminos allocated by init.
 */
void freeTetriminos(tetrimino_t *tetriminos);

/*
 * Rotates the specified tetrimino simply by looking up the positions of the
 * squares in the next orientation stored in the array.
 * @param tetrimino: A pointer to the tetrimino being rotated.
 * @param curr: The current rotation number(index of rotation in the array).
 * @returns: The rotation number after rotation.
 */
int clockwise(tetrimino_t *tetrimino, int curr);

/*
 * Rotates the specified tetrimino simply by looking up the positions of the
 * squares in the next orientation stored in the array.
 * @param tetrimino: A pointer to the tetrimino being rotated.
 * @param curr: The current rotation number(index of rotation in the array).
 * @returns: The rotation number after rotation.
 */
int antiClockwise(tetrimino_t *tetrimino, int curr);

/*
 * Combines two position vectors, and stores the result in the position pointed
 * to by dst.
 */
void pplus(position_t *dst, position_t base, position_t shift);

#endif

