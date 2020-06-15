#include "gpio_sports.h"
#include "sys/time.h"
#include <wiringPi.h>
#include <stdio.h>

/* GPIO 4 - PIN 16 */
#define LEFT_ULTRASOUND 4
/* GPIO 5 - PIN 18 */
#define RIGHT_ULTRASOUND 5
/* GPIO 6 - PIN 22 */
#define RLEFT_IRSENSOR 6
/* GPIO 27 - PIN 27 */
#define RRIGHT_IRSENSOR 27
/* GPIO 25 - PIN 37 */
#define DOWN_SOUND 25
/* GPIO 0 - PIN 11 */
#define ULTRASOUND_TRIGGER 0

#define ultra_range(time) ((time > 0) && (time < 800))
#define FLAG_COUNT 10

static int left;

/*
 * Detects the distance measured by an ultrasound sensor in centimeter.
 */
static int detect_ultrasound(int ultrasound_sensor);

operator_t get_sports(void) {
  int distance = detect_ultrasound(LEFT_ULTRASOUND);

  if (ultra_range(distance) && (left == 0)){
    left = FLAG_COUNT;
    return LEFT;
  }

  if (!ultra_range(distance) && (left > 0)) {
    left--;
  }
  return NONE;
}

void init_gpio_sp(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }

  /* Initializes all used PINs to their corresponding modes */
  pinMode(LEFT_ULTRASOUND, INPUT);
  pinMode(RIGHT_ULTRASOUND, INPUT);
  pinMode(RLEFT_IRSENSOR, INPUT);
  pinMode(RRIGHT_IRSENSOR, INPUT);
  pinMode(DOWN_SOUND, INPUT);
  pinMode(ULTRASOUND_TRIGGER, OUTPUT);

  /* Initializes all flag variables to 0 */
  left = 0;
}

static int detect_ultrasound(int ultrasound_sensor) {
  struct timeval start, end;
  
  digitalWrite(ULTRASOUND_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND_TRIGGER, LOW);

  while (digitalRead(ultrasound_sensor) == LOW)
    gettimeofday(&start, NULL);
  while (digitalRead(ultrasound_sensor) == HIGH);
    gettimeofday(&end, NULL);
  
  return end.tv_usec - start.tv_usec;
}

