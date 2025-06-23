#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwip/tcp.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

#define JOYSTICK_X_PIN 27
#define JOYSTICK_Y_PIN 26

#define WIFI_SSID     "YOUR_WIFI_SSID"     // Nome da rede Wi-Fi
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD" // Senha da rede Wi-Fi

#define SERVER_IP   "192.168.2.59" // IPV4 do computador (ou servidor)
#define SERVER_PORT 80

#define API_ENDPOINT "/api"

typedef struct { 
    struct tcp_pcb *tcp_pcb;
    char *data;
    u16_t len;
    bool connected;
} tcp_connection_t;

bool button_a = false;
bool button_b = false;

float celsius_temperature = 0.0f;

uint16_t joystick_x = 0;     
uint16_t joystick_y = 0;

char wind_rose[14] = { 0 };

float get_internal_temperature(void) {
    adc_select_input(4);

    uint16_t raw = adc_read();
    float voltage = raw * 3.3f / 4095.0f;
    float celsius_temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

    return celsius_temperature;
}

const char *get_wind_rose(int x, int y) {
    if (x > 3000 && y > 3000) return "Nordeste (NE)";
    if (x > 3000 && y < 1000) return "Sudeste (SE)";
    if (x < 1000 && y > 3000) return "Noroeste (NO)";
    if (x < 1000 && y < 1000) return "Sudoeste (SO)";

    if (x > 3000) return "Leste (E)";
    if (x < 1000) return "Oeste (O)";
    if (y > 3000) return "Norte (N)";
    if (y < 1000) return "Sul (S)";

    return "Centro";
}

static err_t tcp_recv_callback(void *args, struct tcp_pcb *tcp_pcb, struct pbuf *pbuf, err_t err) {
    if (!pbuf) {
        tcp_close(tcp_pcb);
        tcp_connection_t *connection = (tcp_connection_t *)args;
        mem_free(connection->data);
        mem_free(connection);

        return ERR_OK;
    }
    
    printf("Response:\n%.*s\n", pbuf->len, (char *)pbuf->payload);
    tcp_recved(tcp_pcb, pbuf->tot_len);
    pbuf_free(pbuf);

    return ERR_OK;
}

static err_t tcp_sent_callback(void *args, struct tcp_pcb *tcp_pcb, u16_t len) {
    printf("Data sent (%u bytes)\n", len);
    return ERR_OK;
}

void tcp_error_callback(void *arg, err_t err) {
    printf("Error sending data: %d\n", err);
}

static err_t tcp_connected_callback(void *args, struct tcp_pcb *tcp_pcb, err_t err) {
    tcp_connection_t *connection = (tcp_connection_t *)args;

    if (err != ERR_OK) {
        printf("Connection error: %d\n", err);

        tcp_abort(tcp_pcb);
        mem_free(connection->data);
        mem_free(connection);

        return err;
    }

    connection->connected = true;

    tcp_recv(tcp_pcb, tcp_recv_callback);
    tcp_sent(tcp_pcb, tcp_sent_callback);

    err_t tcp_write_status = tcp_write(tcp_pcb, connection->data, connection->len, TCP_WRITE_FLAG_COPY);

    if (tcp_write_status != ERR_OK) {
        printf("Error writing TCP data: %d\n", tcp_write_status);

        tcp_abort(tcp_pcb);
        mem_free(connection->data);
        mem_free(connection);

        return tcp_write_status;
    }

    tcp_output(tcp_pcb);

    return ERR_OK;
}

void read_sensor_data(void) {
    button_a = !gpio_get(BUTTON_A_PIN);
    button_b = !gpio_get(BUTTON_B_PIN);

    celsius_temperature = get_internal_temperature();

    adc_select_input(1);
    joystick_x = adc_read();     
    adc_select_input(0);
    joystick_y = adc_read();
    
    if (joystick_x < 0 || joystick_x > 4095) joystick_x = 0;
    if (joystick_y < 0 || joystick_y > 4095) joystick_y = 0;

    memset(wind_rose, 0, sizeof(wind_rose));
    strncpy(wind_rose, get_wind_rose(joystick_x, joystick_y), sizeof(wind_rose));
}

void request(const char *method, const char *server_ip, u16_t port, const char *endpoint, const char *body) {
    char request[1024];
    int request_length = 0;

    if (strcmp(method, "POST") == 0) {
        if (!body) {
            printf("Error: POST request requires a body.\n");
            return;
        }

        size_t body_length = strlen(body);

        request_length = snprintf(request, sizeof(request),
            "POST %s HTTP/1.1\r\n"
            "Host: %s:%d\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n\r\n"
            "%s",
            endpoint,
            server_ip,
            port,
            body_length,
            body
        );
    }
    else if (strcmp(method, "GET") == 0) {
        request_length = snprintf(request, sizeof(request),
            "GET %s HTTP/1.1\r\n"
            "Host: %s:%d\r\n"
            "Connection: close\r\n\r\n",
            endpoint,
            server_ip,
            port
        );
    }
    else {
        printf("HTTP method not supported: %s\n", method);
        return;
    }

    ip_addr_t ip;
    ipaddr_aton(server_ip, &ip);
    struct tcp_pcb *tcp_pcb = tcp_new();
    if (!tcp_pcb) return;

    tcp_connection_t *connection = (tcp_connection_t *)mem_malloc(sizeof(tcp_connection_t));

    if (!connection) {
        tcp_abort(tcp_pcb);
        return;
    }

    connection->data = (char *)mem_malloc(request_length);
    memcpy(connection->data, request, request_length);

    connection->len = request_length;
    connection->tcp_pcb = tcp_pcb;
    connection->connected = false;

    tcp_arg(tcp_pcb, connection);
    tcp_err(tcp_pcb, tcp_error_callback);

    err_t tcp_connect_status = tcp_connect(tcp_pcb, &ip, port, tcp_connected_callback);

    if (tcp_connect_status != ERR_OK) {
        printf("TCP connection error: %d\n", tcp_connect_status);
        mem_free(connection->data);
        mem_free(connection);
        tcp_abort(tcp_pcb);
    }
}

void send_data_to_api(void) {
    char json[256];
    
    snprintf(json, sizeof(json),
        "{"
            "\"button_a\": %d,"
            "\"button_b\": %d,"
            "\"joystick_x\": %u,"
            "\"joystick_y\": %u,"
            "\"wind_rose\": \"%s\","
            "\"temperature\": %.2f"
        "}",
        button_a,
        button_b,
        joystick_x,
        joystick_y,
        wind_rose,
        celsius_temperature
    );

    printf("Body:\n%s\n", json);

    request("POST", SERVER_IP, SERVER_PORT, API_ENDPOINT, json);
}

void setup(void) {
    stdio_init_all();

    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);

    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);

    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    adc_init();

    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    adc_set_temp_sensor_enabled(true);
}

void connect_to_wifi(void) {
    if (cyw43_arch_init()) {
        printf("Error initializing Wi-Fi.\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000) != 0) {
        printf("Trying to connect to Wi-Fi...\n");
        sleep_ms(3000);
    }

    printf("Connected to Wi-Fi!\n");
    printf("IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
}

int main(void) {  
    setup();
    connect_to_wifi();

    while (true) {
        read_sensor_data();
        send_data_to_api(); 
        sleep_ms(1000);
    }

    cyw43_arch_deinit();

    return 0;
}