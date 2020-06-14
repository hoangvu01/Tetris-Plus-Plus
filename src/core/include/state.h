#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "tetriminos.h"
#include "levels.h"


typedef struct {
	/* list of all tetrimino and grid of static blocks */
	tetrimino_t *list;
	grid_t grid;
	level_t level;
	int totalLines;
	int highScore;
	/* information about active block */
	tetrimino_t *nextBlock;
  tetrimino_t *block;
  position_t pos;
  int rotation;
} state_t;

state_t *initState();
void freeState(state_t *curr);
void detectInput(state_t *curr);
void pauseGame();
bool canMove(state_t *teststate);
bool spawnTetriminos(state_t *curr);
bool dropPiece(state_t *curr);
int readHighScore();
void writeHighScore(int highScore);
#endif
