#include "include/sensors.h"

bool button_a = false;
bool button_b = false;
float celsius_temperature = 0.0f;

ssd1306_t display;

void setup(void) {
    stdio_init_all();

    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_init(GREEN_LED_PIN);

    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    i2c_init(i2c1, 400000);
    gpio_set_function(SDA_DISPLAY_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_DISPLAY_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_pull_up(SDA_DISPLAY_PIN);
    gpio_pull_up(SCL_DISPLAY_PIN);

    adc_init();
    adc_set_temp_sensor_enabled(true);

    display.external_vcc = false;
    
    ssd1306_init(&display, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&display);
    ssd1306_show(&display);
}

void read_sensor_data(void) {
    button_a = !gpio_get(BUTTON_A_PIN);
    button_b = !gpio_get(BUTTON_B_PIN);
    celsius_temperature = get_internal_temperature();
}

void manages_led_event(const char *message) {
    if (!strcmp(message, MQTT_MESSAGE_LED_ON)) {
        gpio_put(GREEN_LED_PIN, true);
        printf("LED switched on by MQTT.\n");
    }
    else if (!strcmp(message, MQTT_MESSAGE_LED_OFF)) {
        gpio_put(GREEN_LED_PIN, false);
        printf("LED switched off by MQTT.\n");
    }
    else {
        printf("LED not triggered due to invalid MQTT message.\n");
    }
}

static float get_internal_temperature(void) {
    adc_select_input(4);

    uint16_t raw = adc_read();
    float voltage = raw * 3.3f / 4095.0f;
    float celsius_temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

    return celsius_temperature;
}