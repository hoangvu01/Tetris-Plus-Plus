/*
 * An Interface for using buttons to operate Tetris
 */

#ifndef GPIO_BUT
#define GPIO_BUT

#include "gpio_input.h"

operator_t get_button(void);
void init_gpio_but(void);

#endif
