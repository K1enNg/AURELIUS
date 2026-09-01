#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "status_led.h"

void main() {
    status_led_config_t led_config = {
        .gpio_num = GPIO_NUM_2, 
        .active_high = true      // Set to true if the LED is active high, false if active low
    };

    status_led_init(&led_config);

    while(1) {
        status_led_toggle();
        vTaskDelay(pdMS_TO_TICKS(500)); // Toggle every 1 second
    }
}