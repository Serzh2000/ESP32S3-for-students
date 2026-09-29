#include "driver/rtc_io.h"                  // функции rtc_gpio_*

rtc_gpio_pullup_en(GPIO_NUM_5);             // подтяжка, работающая во сне
rtc_gpio_pulldown_dis(GPIO_NUM_5);
esp_sleep_enable_ext0_wakeup(GPIO_NUM_5, 0); // проснуться при LOW (кнопка нажата)
esp_deep_sleep_start();
