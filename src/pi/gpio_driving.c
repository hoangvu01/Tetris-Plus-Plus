/* Tetris driving is not presented due to mulfunctioning sensors. */

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include "gpio_driving.h"

/* GPIO 8 (I2C) - PIN 3 */
#define LIGHT_INTENSITY_SDA 8
/* GPIO 9 (I2C) - PIN 5 */
#define LIGHT_INTENSITY_SCL 9

/* Constants for GY-30 Light Intensity Sensor */
#define ADDR_WRITE 0x46
#define ADDR_READ 0x47
#define POWER_DOWN 0x00
#define POWER_ON 0x01
#define RESET 0x07
/* Low Resolution Sampling (4lx) */
#define MODE_LOW 0x13
/* High Resolution Sampling (1lx) */
#define MODE_HIGH 0x10
#define DEV_ADDR 0x23

/* ID for the I2C device on a Respberry PI 3b */
#define DEVICE 1

static int fd = 0;

operator_t get_driving(void) {
  int res = wiringPiI2CReadReg8(fd, ADDR_READ);
  printf("%d\n", res);
  return NONE;
}

void init_gpio_dr(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }
  
  /* Initializes the I2C system */
  fd = wiringPiI2CSetup(DEV_ADDR);
  printf("%d\n", fd);
  printf("%d\n", wiringPiI2CWriteReg8(fd, ADDR_WRITE, POWER_ON));
  wiringPiI2CWriteReg8(fd, ADDR_WRITE, RESET);
  wiringPiI2CWriteReg8(fd, ADDR_WRITE, MODE_LOW);
  delay(1000);
}
