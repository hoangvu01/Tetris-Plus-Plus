/* Implements the functions that fetch operation from PI's GPIO ports */

#include <stdio.h>
#include "gpio_input.h"
#include "gpio_button.h"
#include "gpio_sports.h"
#include "gpio_driving.h"
#include "gpio_rhythms.h"
#include "gpio_led_array.h"

/* Total number of input modes that are currently configured */
#define MODE_NUM 5

/* Function pointers arrrays used to simplify the code */
typedef operator_t (*gpio_inputs) (void);
typedef void (*gpio_inits) (void);
gpio_inputs input_selector[] = {NULL, get_button, get_sports, get_driving, get_rhythms};
gpio_inits init_selector[] = {NULL, init_gpio_but, init_gpio_sp, init_gpio_dr, init_gpio_ry};


operator_t gpio_input(inmode_t mode) {
  if ((mode == KEY_BOARD) || (mode >= MODE_NUM)) {
    fprintf(stderr, "Unknown input mode!");
    return NONE;
  }
  return input_selector[mode]();
}

void init_gpio(inmode_t mode) {
  if ((mode == KEY_BOARD) || (mode >= MODE_NUM)) {
    fprintf(stderr, "Unknown input mode!");
    return;
  }
  init_selector[mode]();
  init_led_arr();
}
