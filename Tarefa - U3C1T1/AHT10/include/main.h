#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"
#include "aht10.h"

#define HUMIDITY_THRESHOLD    70.0f
#define TEMPERATURE_THRESHOLD 20.0f

bool check_alert_conditions(float temperature, float humidity);
void display_alert(float temperature, float humidity);
void display_normal_info(float temperature, float humidity);

#endif // MAIN_H