/*
 * An Interface for handling inputs of Tetris DRIVING mode
 */

#ifndef GPIO_DRIVING
#define GPIO_DRIVING

#include "gpio_input.h"

operator_t get_driving(void);
void init_gpio_dr(void);

#endif