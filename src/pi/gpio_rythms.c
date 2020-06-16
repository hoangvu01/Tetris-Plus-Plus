#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_rythms.h"

/* Device Address for the LEFT Controller */
#define LEFT_DEV_ADDR 0x68
/* Device Address for the RIGHT Controller */
/* AD0 PIN for the RIGHT Controller should be connected to VCC */
#define RIGHT_DEV_ADDR 0x69

/* MPU6050 Accelerometer Registers Addresses */
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

#define accel_thres(acc) ((acc < -10000) || (acc > 10000))
#define gyro_thres(gacc) ((gacc < -10000) || (gacc > 10000))

static int fd_l, fd_r;
static short read_data(int fd, int addr);

operator_t get_rythms(void) {
  /* Reads data from  */
  int lx = read_data(fd_l, ACCEL_X);
  int ly = read_data(fd_l, ACCEL_Y);
  int lz = read_data(fd_l, ACCEL_Z);
  int rx = read_data(fd_r, ACCEL_X);
  int ry = read_data(fd_r, ACCEL_Y);
  int rz = read_data(fd_r, ACCEL_Z);
  int lrz = read_data(fd_l, GYRO_Z);
  int rrz = read_data(fd_r, GYRO_Z);

  if (accel_thres(ly)) {
    return LEFT;
  }
  if (accel_thres(ry)) {
    return RIGHT;
  }
  if (gyro_thres(lrz)) {
    return RLEFT;
  }
  if (gyro_thres(rrz)) {
    return RRIGHT;
  }
  if (accel_thres(lx) && accel_thres(rx)) {
    return DOWN;
  }
  if (accel_thres(lz) && accel_thres(rz)) {
    return PAUSE;
  }
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
