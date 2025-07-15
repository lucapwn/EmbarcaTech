#include "include/main.h"

int main(void) {
    setup();
    connect_to_wifi();

    MQTT_CLIENT_T *client = mqtt_client_init();

    create_mqtt_client(client);
    start_mqtt_connection(client);
    start_mqtt_subscription(client);

    while (true) {
        read_sensor_data();
        start_mqtt_publishing(client);
    }

    cyw43_arch_deinit();

    return 0;
}