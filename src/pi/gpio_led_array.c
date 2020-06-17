#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

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

static uint8_t data[4] = {0x0, 0x0, 0x0, 0x0};
static uint8_t heart[8] = {0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18};


void *display_led_up(void *dummy) {
  while (1) {
    for (int j = 0; j < 8; j++) {
      data[0] = ~heart[j];
      data[1] = 0xFF;
      data[2] = 0xFF;
      data[3] = 0x01 << j;
      wiringPiSPIDataRW(UPPER_DISPLAY, data, sizeof(data));
      delay(OUT_DELAY);
    }
  }
}

void *display_led_low(void *dummy) {
  while (1) {
    for (int j = 0; j < 8; j++) {
      data[0] = heart[j];
      data[1] = 0xFF;
      data[2] = 0xFF;
      data[3] = 0x01 << j;
      wiringPiSPIDataRW(LOWER_DISPLAY, data, sizeof(data));
      delay(OUT_DELAY);
    }
  }
}

void init_led_arr(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }
  wiringPiSPISetup(UPPER_DISPLAY, SPI_SPEED);
  wiringPiSPISetup(LOWER_DISPLAY, SPI_SPEED);

  /* Creates a separate thread for the display */
  pthread_t thread_id_u, thread_id_l;
  pthread_create(&thread_id_u, NULL, display_led_up, NULL);
  pthread_create(&thread_id_l, NULL, display_led_low, NULL);
}

int main(void) {
  init_led_arr();
  printf("This keeps running\n");
  sleep(5);
  heart[0] = 0xFF;
  sleep(5);
}
