#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct {
    gpio_num_t gpio_num;
    bool active_high;
} status_led_config_t;                                                                                                                                                                                                          

esp_err_t status_led_init(const status_led_config_t *config);
esp_err_t status_led_set_state(bool on);
esp_err_t status_led_toggle(void);

#endif