#include "include/mqtt.h"

extern bool button_a;
extern bool button_b;
extern float celsius_temperature;
extern ssd1306_t display;

static uint32_t last_time = 0;

MQTT_CLIENT_T *mqtt_client_init(void) {
    MQTT_CLIENT_T *client = calloc(1, sizeof(MQTT_CLIENT_T));

    if (!client) {
        printf("Failed to allocate memory for the MQTT client.\n");
        return NULL;
    }

    return client;
}

static err_t connects_to_mqtt_client(MQTT_CLIENT_T *client) {
    struct mqtt_connect_client_info_t ci = {0};
    ci.client_id = "Raspberry Pi Pico W";
    return mqtt_client_connect(client->mqtt_client, &(client->remote_addr), MQTT_SERVER_PORT, mqtt_connection_callback, client, &ci);
}

static void run_dns_lookup(MQTT_CLIENT_T *client) {
    printf("Running DNS lookup for %s\n", MQTT_SERVER_HOST);
    
    if (dns_gethostbyname(MQTT_SERVER_HOST, &(client->remote_addr), dns_found, client) == ERR_INPROGRESS) {
        while (ip_addr_isany(&(client->remote_addr))) {
            cyw43_arch_poll();
            sleep_ms(10);
        }
    }
}

static void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    MQTT_CLIENT_T *client = (MQTT_CLIENT_T*)callback_arg;

    if (!ipaddr) {
        printf("DNS resolution failure.\n");
        return;
    }

    client->remote_addr = *ipaddr;
    printf("DNS resolved: %s\n", ip4addr_ntoa(ipaddr));
}

void create_mqtt_client(MQTT_CLIENT_T *client) {
    run_dns_lookup(client);
    
    client->mqtt_client = mqtt_client_new();

    if (!client->mqtt_client) {
        printf("Failed to create MQTT client.\n");
        return;
    }
}

void start_mqtt_connection(MQTT_CLIENT_T *client) {
    if (connects_to_mqtt_client(client) != ERR_OK) {
        printf("Failed to connect to the MQTT client.\n");
        return;
    }
}

static void mqtt_connection_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status != MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connection failed: %d\n", status);
        return;
    }

    printf("MQTT connected.\n");

    ssd1306_clear(&display);
    ssd1306_draw_string(&display, 0, 10, 1, "LED on/off by MQTT");
    ssd1306_draw_string(&display, 0, 30, 1, "Send " MQTT_MESSAGE_LED_OFF " or " MQTT_MESSAGE_LED_ON " (on/off)");
    ssd1306_draw_string(&display, 0, 50, 1, "Nothing received.");
    ssd1306_show(&display);
}

static void mqtt_publish_start_callback(void *arg, const char *topic, u32_t tot_len) {
    printf("Message received on topic: %s\n", topic);
}

static void mqtt_publish_data_callback(void *arg, const u8_t *data, u16_t length, u8_t flags) {
    if (length >= MQTT_BUFFER_SIZE) {
        printf("The received message is larger than the buffer.\n");
        return;
    }

    char buffer[length];
    char message[MQTT_BUFFER_SIZE];

    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(message));
    memcpy(buffer, data, sizeof(buffer));

    buffer[length] = '\0';
    message[MQTT_BUFFER_SIZE - 1] = '\0';

    snprintf(message, sizeof(message), "Message: %s", buffer);

    ssd1306_clear(&display);
    ssd1306_draw_string(&display, 0, 10, 1, "LED on/off by MQTT");
    ssd1306_draw_string(&display, 0, 30, 1, "Send " MQTT_MESSAGE_LED_OFF " or " MQTT_MESSAGE_LED_ON " (on/off)");
    ssd1306_draw_string(&display, 0, 50, 1, message);
    ssd1306_show(&display);

    printf("Message received: %s\n", buffer);

    manages_led_event(buffer);
}

void start_mqtt_subscription(MQTT_CLIENT_T *client) {
    mqtt_set_inpub_callback(client->mqtt_client, mqtt_publish_start_callback, mqtt_publish_data_callback, NULL);
    err_t subscribe_return = mqtt_sub_unsub(client->mqtt_client, DEVICE_MESSAGE_MQTT_TOPIC, 2, NULL, NULL, 1);

    if (subscribe_return != ERR_OK) {
        printf("Failed to subscribe to the \"%s\" topic.\n", DEVICE_MESSAGE_MQTT_TOPIC);
        return;
    }

    printf("Successful subscription to the \"%s\" topic.\n", DEVICE_MESSAGE_MQTT_TOPIC);
}

void start_mqtt_publishing(MQTT_CLIENT_T *client) {
    cyw43_arch_poll();

    if (!mqtt_client_is_connected(client->mqtt_client)) {
        printf("Trying to connect to the MQTT client.\n");
        connects_to_mqtt_client(client);
    }

    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (now - last_time >= PUBLISH_DELAY_MS) {
        char json[MQTT_BUFFER_SIZE];
    
        snprintf(json, sizeof(json),
            "{"
                "\"button_a\": %d, "
                "\"button_b\": %d, "
                "\"temperature\": %.2f"
            "}",
            button_a,
            button_b,
            celsius_temperature
        );

        err_t publish_return = mqtt_publish(client->mqtt_client, SERVER_MESSAGE_MQTT_TOPIC, json, strlen(json), 0, 0, NULL, client);

        if (publish_return != ERR_OK) {
            printf("Failed to publish to the \"%s\" topic.\n", SERVER_MESSAGE_MQTT_TOPIC);
            return;
        }

        printf("Successful publishing to the \"%s\" topic.\n%s\n", SERVER_MESSAGE_MQTT_TOPIC, json);
        last_time = now;
    }
}