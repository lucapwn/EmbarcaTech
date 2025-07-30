#ifndef MPU6050_H
#define MPU6050_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdint.h>
#include <math.h>

#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

#define MPU6050_ADDR 0x68

void mpu6050_init(void);
void mpu6050_read_raw(int16_t *ax, int16_t *ay, int16_t *az);
float mpu6050_get_inclination(int16_t ax, int16_t ay, int16_t az);

#endif // MPU6050_H