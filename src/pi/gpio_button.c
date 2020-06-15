#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include "gpio_button.h"

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

/* Check whether all buttons are not pressed */
static bool all_low(void);
/* Flag that avoids duplicated inputs */
static bool set;

operator_t get_button(void) {
  /* If the previous call provides an input, return NONE to avoid duplicating input */
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
  /* Clear set flag to allow duplicated inputs from DOWN_BUTTON */
  set = false;
  if (digitalRead(DOWN_BUT) == HIGH) {
    return DOWN;
  }
  return NONE;
}

void init_gpio_but(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }

  set = false;

  /* Initializes all used PINs to INPUT mode and pull down internal resistors */
  pinMode(LEFT_BUT, INPUT);
  pullUpDnControl(LEFT_BUT, PUD_DOWN);
  pinMode(RIGHT_BUT, INPUT);
  pullUpDnControl(RIGHT_BUT, PUD_DOWN);
  pinMode(RLEFT_BUT, INPUT);
  pullUpDnControl(RLEFT_BUT, PUD_DOWN);
  pinMode(RRIGHT_BUT, INPUT);
  pullUpDnControl(RRIGHT_BUT, PUD_DOWN);
  pinMode(DOWN_BUT, INPUT);
  pullUpDnControl(DOWN_BUT, PUD_DOWN);
}

static bool all_low(void) {
  return (digitalRead(LEFT_BUT) == LOW) && (digitalRead (RIGHT_BUT) == LOW)
      && (digitalRead(RLEFT_BUT) == LOW) && (digitalRead(RRIGHT_BUT) == LOW)
      && (digitalRead(DOWN_BUT) == LOW);
}
