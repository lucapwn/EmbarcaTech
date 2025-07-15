#include "include/wifi.h"

void connect_to_wifi(void) {
    sleep_ms(1000);

    if (cyw43_arch_init()) {
        printf("Failed to initialize the Wi-Fi module.\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000) != 0) {
        printf("Failed to connect to Wi-Fi.\n");
        return;
    }

    printf("Connected to Wi-Fi!\n");
    printf("IP Address: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
}