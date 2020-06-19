#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>
#include "gpio_sports.h"
#include <sys/time.h>

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

/* Time interval for ultrasound reflection that is considered in range*/
#define ultra_range(time) ((time > 0) && (time < 1000))
/* Special flag used to avoid duplicated inputs. Optimized for the behavior of sensors */
#define FLAG_COUNT 10

/* Specifies which sensors are plugged in */
#define LEFT_PLUG_IN
#define RIGHT_PLUG_IN
#define RLEFT_PLUG_IN
#define SOUND_PLUG_IN
// #define ALL_PLUG_IN

/* Flags used to avoid duplicated inputs */
static int left, right, rleft, rright, down;

/*
 * Detects the distance measured by an ultrasound sensor
 * @param ultrasound_sensor: PIN number for the ultrasound sensor
 * @returns: Time taken for the ultrasound to travel and bounce back (in microsecond)
 */
static int detect_ultrasound(int ultrasound_sensor);
static void *detect_sound(void *dummy); 

operator_t get_sports(void) {
  if (down > 0) {
    down--;
    return DOWN;
  }

  #if defined LEFT_PLUG_IN || defined ALL_PLUG_IN
    int distance_l = detect_ultrasound(LEFT_ULTRASOUND);
    if (ultra_range(distance_l) && (left == 0)) {
      left = FLAG_COUNT;
      return LEFT;
    }
    if (!ultra_range(distance_l) && (left > 0)) {
      left--;
    }
  #endif

  #if defined RIGHT_PLUG_IN || defined ALL_PLUG_IN
    int distance_r = detect_ultrasound(RIGHT_ULTRASOUND);
    if (ultra_range(distance_r) && (right == 0)) {
      right = FLAG_COUNT;
      return RIGHT;
    }
    if (!ultra_range(distance_r) && (right > 0)) {
      right--;
    }
  #endif

  #if defined RLEFT_PLUG_IN || defined ALL_PLUG_IN
    int ir_left = digitalRead(RLEFT_IRSENSOR);
    if ((ir_left == HIGH) && (rleft == 0)) {
      rleft = 1;
      return RLEFT;
    }
    if ((ir_left == LOW) && (rleft > 0)) {
      rleft = 0;
    }
  #endif

  #if defined RRIGHT_PLUG_IN || defined ALL_PLUG_IN
    int ir_right = digitalRead(RRIGHT_IRSENSOR);
    if ((ir_right == HIGH) && (rright == 0)) {
      rright = 1;
      return RRIGHT;
    }
    if ((ir_right == LOW) && (rright > 0)) {
      rright = 0;
    }
  #endif
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
  right = 0;
  rleft = 0;
  rright = 0;
  down = 0;

  #if defined SOUND_PLUG_IN || defined ALL_PLUG_IN
    /* Start a separate thread to monitor the sound sensor */
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, detect_sound, NULL);
  #endif
}

static int detect_ultrasound(int ultrasound_sensor) {
  struct timeval start, end;
  
  /* Sends an ultrasound signal  */
  digitalWrite(ULTRASOUND_TRIGGER, HIGH);
  delayMicroseconds(20);
  digitalWrite(ULTRASOUND_TRIGGER, LOW);

  /* Detects the ultrasound signal and calculates delta time */
  while (digitalRead(ultrasound_sensor) == LOW)
    gettimeofday(&start, NULL);
  while (digitalRead(ultrasound_sensor) == HIGH);
    gettimeofday(&end, NULL);
  
  return end.tv_usec - start.tv_usec;
}

static void *detect_sound(void *dummy) {
  while (1) {
    int sound = digitalRead(DOWN_SOUND);
    if (sound == LOW) {
      down = 5;
      delay(1000);
    }   
  }
  return NULL;
}
