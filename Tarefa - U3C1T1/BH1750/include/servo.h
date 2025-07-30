#ifndef SERVOR_H
#define SERVOR_H

#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#define SERVO_PIN 2

#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500

#define PWM_PERIOD_MS 20

void servo_init();
void servo_set_angle(uint angle);

#endif // SERVOR_H