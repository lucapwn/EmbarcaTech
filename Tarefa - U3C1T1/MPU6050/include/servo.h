#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define SERVO_PIN 28

#define ANGLE_ALERT_THRESHOLD 60

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500

#define PWM_PERIOD_MS 20

static uint32_t angle_to_pulse_width(uint angle);
void servo_init(void);
void servo_set_angle(uint angle);
uint map_servo_angle(int angle);

#endif // SERVO_H