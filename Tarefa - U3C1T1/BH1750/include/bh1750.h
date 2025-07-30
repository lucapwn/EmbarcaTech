#ifndef BH1750_H
#define BH1750_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

#define BH1750_ADDR 0x23
#define BH1750_POWER_ON 0x01
#define BH1750_CONT_HIGH_RES_MODE 0x10

i2c_inst_t* bh1750_init(i2c_inst_t *i2c_instance);
void bh1750_set_mode(i2c_inst_t *i2c, uint8_t mode);
void bh1750_read_lux(i2c_inst_t *i2c, float *lux);
uint map_lux_to_angle(float lux);

#endif // BH1750_H