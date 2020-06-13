/* Implements the functions that fetch operation from PI's GPIO ports */

#include "gpio_input.h"
#include "gpio_button.h"

operator_t get_input(void) {
  init_gpio_but();
  return get_button();
}
