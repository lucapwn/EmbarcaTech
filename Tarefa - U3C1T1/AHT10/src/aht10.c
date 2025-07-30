#include "aht10.h"

void aht10_i2c_init(void) {
    i2c_init(i2c0, 400 * 1000);

    gpio_set_function(AHT10_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(AHT10_I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(AHT10_I2C_SDA_PIN);
    gpio_pull_up(AHT10_I2C_SCL_PIN);
}

void aht10_init(void) {
    uint8_t cmd[] = { 0xBE, 0x08, 0x00 };
    i2c_write_blocking(AHT10_I2C_PORT, AHT10_ADDR, cmd, 3, false);
    sleep_ms(50);
}

void aht10_trigger_measurement(void) {
    uint8_t cmd[] = { 0xAC, 0x33, 0x00 };
    i2c_write_blocking(AHT10_I2C_PORT, AHT10_ADDR, cmd, 3, false);
    sleep_ms(80);
}

bool aht10_read(float *temperature, float *humidity) {
    uint8_t data[6];
    int status = i2c_read_blocking(AHT10_I2C_PORT, AHT10_ADDR, data, 6, false);

    if (status != 6) {
        return false;
    }

    if (data[0] & 0x80) {
        return false;
    }

    uint32_t humidity_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t temperature_raw = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    *humidity = ((float)humidity_raw / 1048576.0f) * 100.0f;
    *temperature = ((float)temperature_raw / 1048576.0f) * 200.0f - 50.0f;

    return true;
}