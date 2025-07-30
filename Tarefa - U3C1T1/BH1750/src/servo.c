#include "servo.h"

static uint slice_num;
static uint channel_num;
static uint32_t wrap_value;

static uint32_t angle_to_pulse_width(uint angle) {
    uint32_t pulse = (angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) / 180) + SERVO_MIN_PULSE_US;
    return pulse;
}

void servo_init() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    channel_num = pwm_gpio_to_channel(SERVO_PIN);

    pwm_config config = pwm_get_default_config();
    float div = (float)clock_get_hz(clk_sys) / 1000000;
    pwm_config_set_clkdiv(&config, div);
    
    wrap_value = PWM_PERIOD_MS * 1000;
    pwm_config_set_wrap(&config, wrap_value);
    pwm_init(slice_num, &config, true);
}

void servo_set_angle(uint angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    uint32_t pulse = angle_to_pulse_width(angle);
    pwm_set_chan_level(slice_num, channel_num, pulse);
}