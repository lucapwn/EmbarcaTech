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

QueueHandle_t button_queue;
QueueHandle_t led_queue;

void vTaskButtonRead(void *args) {
    while (true) {
        button_state_t button_state = gpio_get(BUTTON_PIN) ? BUTTON_RELEASED : BUTTON_PRESSED;
        xQueueSend(button_queue, &button_state, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTaskButtonProcess(void *args) {
    button_state_t button_state;

    while (true) {
        if (xQueueReceive(button_queue, &button_state, portMAX_DELAY)) {
            xQueueSend(led_queue, &button_state, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vTaskLedControl(void *args) {
    button_state_t button_state;

    while (true) {
        if (xQueueReceive(led_queue, &button_state, portMAX_DELAY)) {
            gpio_put(LED_PIN, button_state);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

int main(int argc, char *argv[]) {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    button_queue = xQueueCreate(5, sizeof(button_state_t));
    led_queue = xQueueCreate(5, sizeof(button_state_t));

    xTaskCreate(vTaskButtonRead, "vTaskButtonRead", 128, NULL, 1, NULL);
    xTaskCreate(vTaskButtonProcess, "vTaskButtonProcess", 128, NULL, 1, NULL);
    xTaskCreate(vTaskLedControl, "vTaskLedControl", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {
        tight_loop_contents();
    }

    return 0;
}