#include "status_led.h"
#include "esp_log.h"

static const char *TAG = "STATUS_LED";
static status_led_config_t s_led_config;
static bool s_is_initialized = false;
static bool s_led_state = false;

esp_err_t status_led_init (const status_led_config_t *config) {
    if (!config) {
        ESP_LOGE(TAG, "Invalid configuration");
        return ESP_ERR_INVALID_ARG;
    }

    s_led_config = *config;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << s_led_config.gpio_num),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    esp_err_t ret = gpio_config(&io_conf);
    if (ret == ESP_OK) {
        s_is_initialized = true;
        status_led_set_state(false); // turn off by default
        ESP_LOGI(TAG, "LED driver initialized on GPIO %d", s_led_config.gpio_num);
    } else {
        ESP_LOGE(TAG, "Failed to configure GPIO: %s", s_led_config.gpio_num);
    }

    return ret;
}

esp_err_t status_led_set_state(bool on) {
    if (!s_is_initialized) {
        ESP_LOGE(TAG, "LED driver not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    s_led_state = on;
    uint32_t level = s_led_config.active_high ? (on ? 1 : 0) : (on ? 0 : 1);
    return gpio_set_level(s_led_config.gpio_num, level);
}

esp_err_t status_led_toggle(void) {
    return status_led_set_state(!s_led_state);
}