#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "mqtt.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

#define GREEN_LED_PIN 11

#define SDA_DISPLAY_PIN 14
#define SCL_DISPLAY_PIN 15

void setup(void);
void read_sensor_data(void);
void manages_led_event(const char *message);
static float get_internal_temperature(void);

#endif // SENSORS_H