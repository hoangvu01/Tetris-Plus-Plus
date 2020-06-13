#include "tetriminos.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static void setTetriminos(tetrimino_t *tetriminos, int block, int num_spin,
                          orientation_t *spin, ...);

tetrimino_t *initTetrimino() {
  tetrimino_t *tetriminos = calloc(NUM_BLOCKS, sizeof(tetrimino_t));

									/* { { X0, Y0 }, { X1, Y1 }, { X2, Y2 }, { X3, Y3 }, } */
   orientation_t Tu ={ { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0, -1 }, }; // T up
   orientation_t Tr ={ {  0, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, }; // T right
   orientation_t Td ={ { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, }; // T down (spawn)
   orientation_t Tl ={ {  0, -1 }, { -1,  0 }, {  0,  0 }, {  0,  1 }, }; // T left

   orientation_t Jl ={ {  0, -1 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, }; // J left
   orientation_t Ju ={ { -1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, }; // J up
   orientation_t Jr ={ {  0, -1 }, {  1, -1 }, {  0,  0 }, {  0,  1 }, }; // J right
   orientation_t Jd ={ { -1,  0 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, }; // J down (spawn)

   orientation_t Zh ={ { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, }; // Z horizontal (spawn) 
   orientation_t Zv ={ {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, }; // Z vertical

   orientation_t Os ={ { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, }; // O (spawn)

   orientation_t Sh ={ {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, }; // S horizontal (spawn)
   orientation_t Sv ={ {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, }; // S vertical

   orientation_t Lr ={ {  0, -1 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, }; // L right
   orientation_t Ld ={ { -1,  0 }, {  0,  0 }, {  1,  0 }, { -1,  1 }, }; // L down (spawn)
   orientation_t Ll ={ { -1, -1 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, }; // L left
   orientation_t Lu ={ {  1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, }; // L up

   orientation_t Iv ={ {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, }; // I vertical
   orientation_t Ih ={ { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, }; // I horizontal (spawn)


  setTetriminos(tetriminos, TBLOCK, 4, &Td, &Tl, &Tu, &Tr);
  setTetriminos(tetriminos, JBLOCK, 4, &Jd, &Jl, &Ju, &Jr);
  setTetriminos(tetriminos, LBLOCK, 4, &Ld, &Ll, &Lu, &Lr);

  setTetriminos(tetriminos, ZBLOCK, 2, &Zh, &Zv);
  setTetriminos(tetriminos, SBLOCK, 2, &Sh, &Sv);
  setTetriminos(tetriminos, IBLOCK, 2, &Ih, &Iv);

  setTetriminos(tetriminos, OBLOCK, 1, &Os);

  return tetriminos;
}

static void setTetriminos(tetrimino_t *tetriminos, int block, int num_spin,
                          orientation_t *spin, ...) {
  va_list valist;

  tetriminos[block].num_spin = num_spin;
  tetriminos[block].spins = calloc(num_spin, sizeof(orientation_t));

  va_start(valist, spin);
  orientation_t *curr = spin;
  for (int i = 0; i < num_spin; i++) {
    memcpy(tetriminos[block].spins[i], *curr, sizeof(orientation_t));
    curr = va_arg(valist, orientation_t *);

  }
}

void freeTetriminos(tetrimino_t *tetriminos) {
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		free(tetriminos[i].spins);
	}
	free(tetriminos);
}

int clockwise(tetrimino_t *tetriminos, int curr){
  return (curr + 1) % (tetriminos->num_spin);
}

int antiClockwise(tetrimino_t *tetriminos, int curr){
  return (curr + tetriminos->num_spin - 1) % (tetriminos->num_spin);
}

void pplus(position_t *dst, position_t base, position_t shift){
  dst->x = base.x + shift.x;
  dst->y = base.y + shift.y;
}

