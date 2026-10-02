#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define TAG "BME688"
#define ADDR 0x77
#define SDA GPIO_NUM_6
#define SCL GPIO_NUM_7

static i2c_master_dev_handle_t dev;

static esp_err_t wr(uint8_t r, uint8_t v)
{
    uint8_t d[2] = {r, v};
    return i2c_master_transmit(dev, d, 2, 100);
}

static esp_err_t rd(uint8_t r, uint8_t *d, size_t n)
{
    return i2c_master_transmit_receive(dev, &r, 1, d, n, 100);
}

void app_main(void)
{
    i2c_master_bus_config_t bc = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA,
        .scl_io_num = SCL,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bc, &bus));

    i2c_device_config_t dc = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADDR,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &dc, &dev));

    /* Humidity oversampling x1 */
    ESP_ERROR_CHECK(wr(0x72, 0x01));

    /* Temperature/pressure x1 */
    ESP_ERROR_CHECK(wr(0x74, 0x25));

    while (1) {
        ESP_LOGI(TAG, "Starting measurement");

        /* Forced mode */
        ESP_ERROR_CHECK(wr(0x74, 0x25));

        /* Wait for measurement */
        vTaskDelay(pdMS_TO_TICKS(20));

        uint8_t status;

        if (rd(0x1D, &status, 1) != ESP_OK) {
            ESP_LOGE(TAG, "Status read failed");
            continue;
        }

        ESP_LOGI(TAG, "Status = 0x%02X", status);

        /* Read pressure */
        uint8_t p[3];
        if (rd(0x1F, p, 3) != ESP_OK) {
            ESP_LOGE(TAG, "Pressure read failed");
            continue;
        }

        uint32_t pressure =
            ((uint32_t)p[0] << 12) |
            ((uint32_t)p[1] << 4) |
            (p[2] >> 4);

        /* Read temperature */
        uint8_t t[3];
        if (rd(0x22, t, 3) != ESP_OK) {
            ESP_LOGE(TAG, "Temperature read failed");
            continue;
        }

        uint32_t temperature =
            ((uint32_t)t[0] << 12) |
            ((uint32_t)t[1] << 4) |
            (t[2] >> 4);

        /* Read humidity */
        uint8_t h[2];
        if (rd(0x25, h, 2) != ESP_OK) {
            ESP_LOGE(TAG, "Humidity read failed");
            continue;
        }

        uint16_t humidity =
            ((uint16_t)h[0] << 8) |
            h[1];

        ESP_LOGI(TAG, "RAW TEMP : %lu",
                 (unsigned long)temperature);

        ESP_LOGI(TAG, "RAW PRESS: %lu",
                 (unsigned long)pressure);

        ESP_LOGI(TAG, "RAW HUM  : %u",
                 humidity);

        ESP_LOGI(TAG, "----------------");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}