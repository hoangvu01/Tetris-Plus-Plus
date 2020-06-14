#include <stdio.h>
#include <wiringPi.h>
#include "gpio_input.h"

#define INPUT_DELAY 20

int main(void) {
  init_gpio(BUTTON);
  while (1) {
    operator_t input = get_input(BUTTON);
    if (input != NONE) {
      printf("%d\n",input); 
    }
    delay(INPUT_DELAY);
  }  

  return 0;
}
