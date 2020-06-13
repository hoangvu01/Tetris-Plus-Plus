/*
 * An Interface for using Raspberry PI's GPIO input as the operator of Tetris
 */

#ifndef GPIO_IN
#define GPIO_IN

enum operations {NONE, LEFT, RIGHT, RLEFT, RRIGHT, DOWN};
typedef enum operations operator_t;

/*
 * Get the input from Raspberry PI's GPIO input
 * @returns: An enum for the operation
 */
operator_t get_input(void);

#endif
