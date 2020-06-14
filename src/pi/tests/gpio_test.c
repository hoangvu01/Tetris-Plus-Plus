#include <stdio.h>
#include <wiringPi.h>
#include "gpio_input.h"

#define INPUT_DELAY 500

int main(void) {
  init_gpio();
  while (1) {
    printf("%d\n", get_input());
    delay(INPUT_DELAY);
  }  

  return 0;
}
