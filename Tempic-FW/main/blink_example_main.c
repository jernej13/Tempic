#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BLINK_GPIO GPIO_NUM_20
#define BLINK_PERIOD_MS 1000

void app_main(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BLINK_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_set_level(BLINK_GPIO, 0));


    while (1) {
        ESP_ERROR_CHECK(gpio_set_level(BLINK_GPIO, 1));
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));

        ESP_ERROR_CHECK(gpio_set_level(BLINK_GPIO, 0));
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}