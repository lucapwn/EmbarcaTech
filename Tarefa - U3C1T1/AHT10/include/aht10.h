#ifndef AHT10_H
#define AHT10_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define AHT10_I2C_PORT i2c0
#define AHT10_ADDR 0x38
#define AHT10_I2C_SDA_PIN 0
#define AHT10_I2C_SCL_PIN 1

void aht10_i2c_init(void);
void aht10_init(void);
void aht10_trigger_measurement(void);
bool aht10_read(float *temperature, float *humidity);

#endif // AHT10_H