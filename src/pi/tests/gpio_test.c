#include <stdio.h>
#include "gpio_input.h"

int main(void) {
  printf("%d\n", get_input());

  return 0;
}
