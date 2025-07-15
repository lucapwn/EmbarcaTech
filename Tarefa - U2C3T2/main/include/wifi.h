#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "pico/cyw43_arch.h"

#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

void connect_to_wifi(void);

#endif // WIFI_H