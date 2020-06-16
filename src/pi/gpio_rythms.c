#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_rythms.h"

/* Device Address for the LEFT Controller */
#define LEFT_DEV_ADDR 0x68
/* Device Address for the RIGHT Controller */
#define RIGHT_DEV_ADDR 0x69

/* MPU6050 Accelerometer Registers Addresses*/
#define POWER_MNMT 0x6B
#define SAMPLE_RT 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define INT_ENABLE 0x38
#define ACCEL_X 0x3B
#define ACCEL_Y 0x3D
#define ACCEL_Z 0x3F
#define GYRO_X 0x43
#define GYRO_Y 0x45
#define GYRO_Z 0x47

static int fd_l, fd_r;
static short read_data(int fd, int addr);

operator_t get_rythms(void) {
  printf("Left Y:%d; Right Y:%d\n", read_data(fd_l, ACCEL_Y), read_data(fd_r, ACCEL_Y));
  return NONE;
}

void init_gpio_ry(void) {
  if (wiringPiSetup() == -1) {
    fprintf(stderr, "Unable to initialize WiringPI\n");
    exit(EXIT_FAILURE);
  }

  fd_l = wiringPiI2CSetup(LEFT_DEV_ADDR);
  fd_r = wiringPiI2CSetup(RIGHT_DEV_ADDR);

  /* Sets Sample Rate */
  wiringPiI2CWriteReg8(fd_l, SAMPLE_RT, 0x07);
  wiringPiI2CWriteReg8(fd_r, SAMPLE_RT, 0x07);
  /* Sets Power Management */
  wiringPiI2CWriteReg8(fd_l, POWER_MNMT, 0x01);
  wiringPiI2CWriteReg8(fd_r, POWER_MNMT, 0x01);
  /* Sets Configs */
  wiringPiI2CWriteReg8(fd_l, CONFIG, 0x00);
  wiringPiI2CWriteReg8(fd_r, CONFIG, 0x00);
  /* Sets Gyro Configs */
  wiringPiI2CWriteReg8(fd_l, GYRO_CONFIG, 0x18);
  wiringPiI2CWriteReg8(fd_r, GYRO_CONFIG, 0x18);
  /* Sets Interrupt Enable */
  wiringPiI2CWriteReg8(fd_l, INT_ENABLE, 0x01);
  wiringPiI2CWriteReg8(fd_r, INT_ENABLE, 0x01);
}

/*
 * Read 16-bits data from the I2C registers
 * @param fd: File-handler for the corresponding controller
 * @param addr: Address of I2C register
 * @returns: Value read from the register
 */
static short read_data(int fd, int addr) {
  /* use short to ensure that the sign is correct */
  short data = 0;
  /* Higher Bits */
  data = wiringPiI2CReadReg8(fd, addr) << 8;
  /* Lower Bits */
  data |= wiringPiI2CReadReg8(fd, addr);
  return data;
}
