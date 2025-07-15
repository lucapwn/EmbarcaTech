#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/apps/mqtt.h"
#include "lwip/altcp_tls.h"
#include "lwip/altcp_tcp.h"
#include "sensors.h"

// broker.hivemq.com -> https://www.hivemq.com/demos/websocket-client/
// broker.emqx.io    -> https://mqttx.app/

#define MQTT_SERVER_HOST "broker.hivemq.com"
#define MQTT_SERVER_PORT 1883
#define MQTT_BUFFER_SIZE 256
#define MQTT_TLS         false

#define MQTT_MESSAGE_LED_ON  "1"
#define MQTT_MESSAGE_LED_OFF "0"

#define SERVER_MESSAGE_MQTT_TOPIC "message/server"
#define DEVICE_MESSAGE_MQTT_TOPIC "message/device"

#define PUBLISH_DELAY_MS 1000

typedef struct MQTT_CLIENT_T_ {
    ip_addr_t remote_addr;
    mqtt_client_t *mqtt_client;
    u32_t received;
    u32_t counter;
    u32_t reconnect;
} MQTT_CLIENT_T;

MQTT_CLIENT_T *mqtt_client_init(void);
static err_t connects_to_mqtt_client(MQTT_CLIENT_T *client);
static void run_dns_lookup(MQTT_CLIENT_T *client);
static void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
void create_mqtt_client(MQTT_CLIENT_T *client);
void start_mqtt_connection(MQTT_CLIENT_T *client);
static void mqtt_connection_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_publish_start_callback(void *arg, const char *topic, u32_t tot_len);
static void mqtt_publish_data_callback(void *arg, const u8_t *data, u16_t length, u8_t flags);
void start_mqtt_subscription(MQTT_CLIENT_T *client);
void start_mqtt_publishing(MQTT_CLIENT_T *client);

#endif // MQTT_H