#include <stdio.h>
#include <wiringpi.h>
#include "gpio_input.h"

int main(void) {
  while (1) {
    printf("%d\n", get_input());
    delay(1000);
  }  

  return 0;
}
