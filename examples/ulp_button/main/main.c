#include <stdio.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "ulp_riscv.h"
#include "ulp_main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");
void app_main(void) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("wake=%d\n", (int)esp_sleep_get_wakeup_cause());
    ESP_ERROR_CHECK(rtc_gpio_init(GPIO_NUM_4));
    ESP_ERROR_CHECK(rtc_gpio_set_direction(GPIO_NUM_4, RTC_GPIO_MODE_INPUT_ONLY));
    ESP_ERROR_CHECK(rtc_gpio_pullup_dis(GPIO_NUM_4));
    ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(GPIO_NUM_4));
    // External 10k pull-up: GPIO4 -> resistor -> 3V3; button GPIO4 -> GND.
    ESP_ERROR_CHECK(ulp_riscv_load_binary(ulp_main_bin_start,
                          ulp_main_bin_end - ulp_main_bin_start));
    ESP_ERROR_CHECK(ulp_set_wakeup_period(0, 100000));
    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
    ESP_ERROR_CHECK(ulp_riscv_run());
    printf("Hold button about one second to wake\n");
    fflush(stdout);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_deep_sleep_start();
}
