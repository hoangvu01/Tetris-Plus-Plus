/*
 * An Interface for using Raspberry PI's GPIO input as the operator of Tetris
 */

#ifndef GPIO_IN
#define GPIO_IN

#include "state.h"

/*
 * Get the input from Raspberry PI's GPIO input
 * @param mode: Sets the type of input from Raspberry PI
 * @returns: An enum for the operation
 */
operator_t gpio_input(inmode_t mode);

/*
 * Initializes the GPIO PINs that are used
 * @param mode: Sets the type of input from Raspberry PI
 */
void init_gpio(inmode_t mode);

#endif
