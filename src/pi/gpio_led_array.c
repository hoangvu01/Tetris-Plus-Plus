#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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


void display_led(void) {
  while (1) {
    for (int j = 0; j < 8; j++) {
      data[0] = ~heart[j];
      data[1] = 0xFF;
      data[2] = 0xFF;
      data[3] = 0x01 << j;
      wiringPiSPIDataRW(0, data, sizeof(data));
      delay(OUT_DELAY);
    }
  }
}

void init_led_arr(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }
  wiringPiSetup();
  wiringPiSPISetup(UPPER_DISPLAY, SPI_SPEED);
  display_led();
}

int main(void) {
  init_led_arr();
}