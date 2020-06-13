#include "gpio_button.h"
#include <stdio.h>
#include <stdbool.h>
#include <wiringpi.h>

/* GPIO 4 - PIN 16 */
#define LEFT_BUT 4
/* GPIO 5 - PIN 18 */
#define RIGHT_BUT 5
/* GPIO 6 - PIN 22 */
#define RLEFT_BUT 6
/* GPIO 27 - PIN 27 */
#define RRIGHT_BUT 27
/* GPIO 25 - PIN 37 */
#define DOWN_BUT 25

static bool all_low(void);
static bool set;

operator_t get_button(void) {
  if (set) {
    set = !all_low();
    return NONE;
  }

  set = true;
  if (digitalRead(LEFT_BUT) == HIGH) {
    return LEFT;
  }
  if (digitalRead(RIGHT_BUT) == HIGH) {
    return RIGHT;
  }
  if (digitalRead(RLEFT_BUT) == HIGH) {
    return RLEFT;
  }
  if (digitalRead(RRIGHT_BUT) == HIGH) {
    return RRIGHT;
  }
  if (digitalRead(DOWN_BUT) == HIGH) {
    return DOWN;
  }
  set = false;
  return NONE;
}

void init_gpio_but(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    return 1;
  }

  set = false;
  pinMode(LEFT_BUT, INPUT);
  pinMode(RIGHT_BUT, INPUT);
  pinMode(RLEFT_BUT, INPUT);
  pinMode(RRIGHT_BUT, INPUT);
  pinMode(DOWN, INPUT);
}

static bool all_low(void) {
  return (digitalRead(LEFT_BUT) == LOW) && (digitalRead (RIGHT_BUT) == LOW)
      && (digitalRead(RLEFT_BUT) == LOW) && (digitalRead(RRIGHT_BUT) == LOW)
      && (digitalRead(DOWN) == LOW)
}