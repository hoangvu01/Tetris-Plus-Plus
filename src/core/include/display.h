#ifndef DISPLAY_H
#define DISPLAY_H

#include "grid.h"
#include "state.h"
#include "gpio_led_array.h"

#define WINDOW_WIDTH 30
#define WINDOW_HEIGHT 30

/*
 * Prints the tetris logo and asks for the starting level from the user.
 * @returns: The sanitised input from the user containing the starting level.
 */
int startScreen();

/*
 * Initialises a new screen containing the actual game.
 * @returns: The pointer to the new game window.
 */
WINDOW *init_display();

/*
 * Prints out the current state of the game for the user to see, alternate
 * implementations must remember to overlay the current active lock on the grid.
 * @param curr: The current state of the game.
 * @param game_window: The pointer to the game window.
 */
void printState(state_t *curr, WINDOW *game_window);

#endif
