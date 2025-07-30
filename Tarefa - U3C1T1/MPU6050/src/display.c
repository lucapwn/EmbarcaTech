#include "display.h"

ssd1306_t display;

void display_init(void) {
    i2c_init(i2c1, 400 * 1000);

    gpio_set_function(DISPLAY_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(DISPLAY_I2C_SDA_PIN);
    gpio_pull_up(DISPLAY_I2C_SCL_PIN);
    
    if (!ssd1306_init(&display, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ADDRESS, i2c1)) {
        printf("Failed to initialize OLED display.\n");
    }
}

void clear_display(void) {
    ssd1306_clear(&display);
}

void show_display(void) {
    ssd1306_show(&display);
}

void display_text_no_clear(const char *text, uint32_t x, uint32_t y, uint32_t scale) {
    ssd1306_draw_string(&display, x, y, scale, text);
}

void display_text(const char *text, uint32_t x, uint32_t y, uint32_t scale) {
    clear_display();
    ssd1306_draw_string(&display, x, y, scale, text);
}