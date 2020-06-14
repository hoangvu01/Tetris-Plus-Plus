#include <stdio.h>
#include <wiringPi.h>
#include "gpio_input.h"

#define INPUT_DELAY 500

int main(void) {
  init_gpio(BUTTON);
  while (1) {
    printf("%d\n", get_input(BUTTON));
    delay(INPUT_DELAY);
  }  

  return 0;
}
