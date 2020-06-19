#include <stdio.h>
#include <wiringPi.h>
#include "gpio_input.h"
#include "state.h"

#define INPUT_DELAY 20
#define TEST_UNIT PI_SPORTS 

int main(void) {
  init_gpio(TEST_UNIT);
  while (1) {
    operator_t input = gpio_input(TEST_UNIT);
    if (input != NONE) {
      printf("%d\n",input); 
    }
    delay(INPUT_DELAY);
  }  

  return 0;
}
