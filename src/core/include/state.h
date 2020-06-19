#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "levels.h"
#include "tetriminos.h"

/* Uncomment this line to enable playing on PI */
// #define PI_MODE
/* Choose mode on this line */
#define GAME_MODE KEY_BOARD

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

enum operations {NONE, LEFT, RIGHT, RLEFT, RRIGHT, DOWN, PAUSE};
typedef enum operations operator_t;
enum modes {KEY_BOARD, PI_BUTTON, PI_SPORTS, PI_DRIVING, PI_RHYTHMS};
typedef enum modes inmode_t;

/*
 * @returns: A heap allocated struct describing the state of the game.
 */
state_t *initState();

/*
 * Clone the current state. This function will produce a deeply cloned copy
 * @return: return the deep cloned state
 */
state_t *cloneState(const state_t *clone);

/*
 * Frees the state_t specified.
 * @param curr: the current state of the game.
 */
void freeState(state_t *curr);

/*
 * Detects keypresses from the user and moves the active tetrimino if allowed by
 * the rules.
 * @param curr: the current state of the game.
 */
void processInput(state_t *curr, inmode_t mode);

/*
 * Pauses the current game and prints out pause message. waits for user input
 * before continuing.
 */
void pauseGame();

/*
 * returns: whether the position of the active block is allowed by the rules.
 * @param teststate: the state to test the legality on. This function does not
 * actually complete the action.
 */
bool canMove(state_t *teststate);

/*
 * returns: whether the a new tetrimino block can be spawned. If false, the game
 * should be terminated.
 * @param curr: the current state of the game.
 */
bool spawnTetriminos(state_t *curr);

/*
 * Drops the active piece by 1 block if able to, otherwise sticks the active
 * block on top, and calls function to clear lines.
 * @param curr: the current state of the game.
 */
bool dropPiece(state_t *curr);

/*
 * reads the high score from disk. 
 * @returns : The high score as an integer. 
 */
int readHighScore();

/*
 * writes the high score to disk. 
 */
void writeHighScore(int highScore);
#endif
