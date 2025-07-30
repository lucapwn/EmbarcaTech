#include "main.h"

int main(void)
{
    stdio_init_all();
    mpu6050_init();
    servo_init();
    display_init();

    int16_t ax = 0;
    int16_t ay = 0;
    int16_t az = 0;

    int angle = 0;
    uint servo_angle = 0;

    char buffer[64];

    while (true) {
        mpu6050_read_raw(&ax, &ay, &az);
        angle = mpu6050_get_inclination(ax, ay, az);

        if (fabs(angle) > ANGLE_ALERT_THRESHOLD) {
            display_text_no_clear("WARNING", 0, 0, 1);
        }
        
        servo_angle = map_servo_angle(angle);

        if (servo_angle < SERVO_MIN_ANGLE) servo_angle = SERVO_MIN_ANGLE;
        if (servo_angle > SERVO_MAX_ANGLE) servo_angle = SERVO_MAX_ANGLE;

        servo_set_angle(servo_angle);

        printf("AX: %d | AY: %d | AZ: %d | Angle: %d° | Servo: %u°\n", ax, ay, az, angle, servo_angle);

        snprintf(buffer, sizeof(buffer), "Angle: %d", angle);
        display_text(buffer, 0, 30, 1);
        show_display();

        sleep_ms(100);
    }

    return 0;
}