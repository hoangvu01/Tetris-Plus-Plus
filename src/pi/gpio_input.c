/* Implements the functions that fetch operation from PI's GPIO ports */

#include "gpio_input.h"
#include "gpio_button.h"

operator_t get_input(void) {
  return get_button();
}

void init_gpio(void) {
  init_gpio_but();
}
