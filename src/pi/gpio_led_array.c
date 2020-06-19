#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "gpio_led_array.h"

/* Clock speed for SPI channels in Hz */
#define SPI_SPEED 500000
/* SPI channel number for upper LED array */
#define UPPER_DISPLAY 0
/* SPI channel number for lower LED array */
#define LOWER_DISPLAY 1
/* Number of rows for the LED array */
#define ROW_NUM 8
/* Output delay for each row in milliseconds */
#define OUT_DELAY 2

#define RED 0
#define BLUE 1
#define GREEN 2

static uint8_t up[8][3];
static uint8_t down[8][3];

static uint8_t to_led_col(colour_t col);
static void *display_led_up(void *dummy);
static void *display_led_low(void *dummy);
static bool updating_up;
static bool updating_down;

void init_led_arr(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }
  wiringPiSPISetup(UPPER_DISPLAY, SPI_SPEED);
  wiringPiSPISetup(LOWER_DISPLAY, SPI_SPEED);
  updating_up = false;
  updating_down = false;

  /* Initializes the grid */
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      up[i][j] = 0;
      down[i][j] = 0;
    }
  }

  /* Creates a separate thread for the display */
  pthread_t thread_id_u, thread_id_l;
  pthread_create(&thread_id_u, NULL, display_led_up, NULL);
  pthread_create(&thread_id_l, NULL, display_led_low, NULL);
}

void update_led(grid_t grid) {
  updating_up = true;
  /* Only updates 8*16 grid */
  /* Updates the upper bit */
  for (int i = 6; i < 14; i++) {
    up[i - 6][RED] = 0x00;
    up[i - 6][BLUE] = 0x00;
    up[i - 6][GREEN] = 0x00;

    for (int j = 1; j < 9; j++) {
      if (grid[i][j] > 0) {
        up[i - 6][to_led_col(grid[i][j])] |= 1 << (j - 1);
      }
    }
  }
  updating_up = false;
  
  updating_down = true;
  /* Updates the lower bit */
  for (int i = 14; i < 22; i++) {
    down[i - 14][RED] = 0x00;
    down[i - 14][BLUE] = 0x00;
    down[i - 14][GREEN] = 0x00;

    for (int j = 1; j < 9; j++) {
      if (grid[i][j] > 0) {
        down[i - 14][to_led_col(grid[i][j])] |= 1 << (j - 1);
      }
    }
  }
  updating_down = false;
}


static void *display_led_up(void *dummy) {
  uint8_t data[4] = {0, 0, 0, 0};
  while (1) {
    for (int i = 0; i < 8; i++) {
      data[RED] = ~up[i][RED];
      data[BLUE] = ~up[i][BLUE];
      data[GREEN] = ~up[i][GREEN];
      data[3] = 0x01 << i;
      if (!updating_up) {
        wiringPiSPIDataRW(UPPER_DISPLAY, data, sizeof(data));
        delay(OUT_DELAY);
      }
    }
  }
  return NULL;
}

static void *display_led_low(void *dummy) {
  uint8_t data[4] = {0, 0, 0, 0};
  while (1) {
    for (int i = 0; i < 8; i++) {
      data[RED] = ~down[i][RED];
      data[BLUE] = ~down[i][BLUE];
      data[GREEN] = ~down[i][GREEN];
      data[3] = 0x01 << i;
      if (!updating_down) {
        wiringPiSPIDataRW(LOWER_DISPLAY, data, sizeof(data));
        delay(OUT_DELAY);
      }
    }
  }
  return NULL;
}

static uint8_t to_led_col(colour_t col) {
  switch (col) {
    case 1:
    case 2:
      return RED;
    case 3:
    case 4:
      return BLUE;
    case 5:
    case 6:
    case 7:
      return GREEN;
    default:
      return RED;    
  }
}
