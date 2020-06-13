/*
 * An Interface for using Raspberry PI's GPIO input as the operator of Tetris
 */

enum operations {NONE, LEFT, RIGHT, RLEFT, RRIGHT};
typedef enum operations operator_t;

/*
 * Get the input from Raspberry PI's GPIO input
 * @returns: An enum for the operation
 */
operator_t get_input(void);
