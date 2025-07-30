#include "main.h"

bool check_alert_conditions(float temperature, float humidity) {
    return (temperature > HUMIDITY_THRESHOLD || humidity < TEMPERATURE_THRESHOLD);
}

void display_alert(float temperature, float humidity) {
    clear_display();

    display_text_no_clear("WARNING", 0, 0, 1);

    if (humidity > HUMIDITY_THRESHOLD && temperature < TEMPERATURE_THRESHOLD) {
        display_text_no_clear("High Humidity", 0, 20, 1);
        display_text_no_clear("Low Temperature", 0, 30, 1);
    }
    else if (humidity > HUMIDITY_THRESHOLD) {
        display_text_no_clear("High Humidity", 0, 20, 1);
    }
    else if (temperature < TEMPERATURE_THRESHOLD) {
        display_text_no_clear("Low Temperature", 0, 20, 1);
    }

    char humidity_str[20];
    char temperature_str[20];

    sprintf(humidity_str, "Humidity: %.2f", humidity);
    sprintf(temperature_str, "Temperature: %.2f", temperature);

    display_text_no_clear(humidity_str, 0, 40, 1);
    display_text_no_clear(temperature_str, 0, 50, 1);
    
    show_display();
}

void display_normal_info(float temperature, float humidity) {
    clear_display();
    
    display_text_no_clear("NORMAL", 0, 0, 1);
    
    char humidity_str[20];
    char temperature_str[20];

    sprintf(humidity_str, "Humidity: %.2f", humidity);
    sprintf(temperature_str, "Temperature: %.2f", temperature);

    display_text_no_clear(humidity_str, 0, 20, 1);
    display_text_no_clear(temperature_str, 0, 30, 1);
    
    show_display();
}

int main(void)
{
    stdio_init_all();
    display_init();
    aht10_i2c_init();
    aht10_init();
    
    float humidity = 0.0f;
    float temperature = 0.0f;

    while (true) {
        aht10_trigger_measurement();

        if (aht10_read(&temperature, &humidity)) {
            if (check_alert_conditions(temperature, humidity)) {
                display_alert(temperature, humidity);
            }
            else {
                display_normal_info(temperature, humidity);
            }
        }
        else {
            printf("Failed to read AHT10 sensor.\n");
        }

        sleep_ms(1000);
    }

    return 0;
}