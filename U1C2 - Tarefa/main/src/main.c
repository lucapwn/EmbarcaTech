#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define BUTTON_PIN  6  // Define o Botão B da BitDogLab
#define LED_PIN    12  // Define o LED azul da BitDogLab

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} button_state_t;

QueueHandle_t buttonQueue;
QueueHandle_t ledQueue;

void vTaskButtonRead(void *args) {
    while (true) {
        button_state_t state = gpio_get(BUTTON_PIN) ? BUTTON_RELEASED : BUTTON_PRESSED;
        xQueueSend(buttonQueue, &state, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTaskButtonProcess(void *args) {
    button_state_t state;

    while (true) {
        if (xQueueReceive(buttonQueue, &state, portMAX_DELAY)) {
            if (state == BUTTON_PRESSED) {
                xQueueSend(ledQueue, &state, 0);
            } else {
                xQueueSend(ledQueue, &state, 0);
            }
        }
    }
}

void vTaskLedControl(void *args) {
    button_state_t command;

    while (true) {
        if (xQueueReceive(ledQueue, &command, portMAX_DELAY)) {
            if (command == BUTTON_PRESSED) {
                gpio_put(LED_PIN, 1);
            } else {
                gpio_put(LED_PIN, 0);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    buttonQueue = xQueueCreate(5, sizeof(button_state_t));
    ledQueue    = xQueueCreate(5, sizeof(button_state_t));

    xTaskCreate(vTaskButtonRead, "button_read", 256, NULL, 1, NULL);
    xTaskCreate(vTaskButtonProcess, "button_process", 256, NULL, 2, NULL);
    xTaskCreate(vTaskLedControl, "led_control", 256, NULL, 2, NULL);

    vTaskStartScheduler();

    while (true) {
        tight_loop_contents();
    }

    return 0;
}