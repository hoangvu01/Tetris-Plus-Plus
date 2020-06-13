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

tetrimino_t *initTetrimino();

void freeTetriminos(tetrimino_t *tetriminos);

int clockwise(tetrimino_t *tetriminos, int curr);

int antiClockwise(tetrimino_t *tetriminos, int curr);

void pplus(position_t *dst, position_t base, position_t shift);

#endif

