#include "main.h"

int main(void)
{
    stdio_init_all();

    i2c_inst_t *i2c = bh1750_init(i2c0);
    servo_init();

    uint angle = 0;
    float lux = 0.0f;

    while (true) {
        bh1750_read_lux(i2c, &lux);
        printf("Luminosity: %.2f lux\n", lux);

        angle = map_lux_to_angle(lux);
        servo_set_angle(angle);
        printf("Servo set to %u°\n", angle);

        sleep_ms(250);
    }
}