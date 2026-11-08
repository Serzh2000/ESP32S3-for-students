# Скетчи к книге

Папка создаётся командой `python export_sketches.py` из `code/*.cpp` — **руками не правьте**:
правки пропадут при следующем запуске. Меняйте `code/*.cpp`, затем запустите скрипт.

## Как пользоваться

**Arduino IDE.** Откройте любую папку `arduino/<урок>/<скетч>/` (файл `.ino`). Плата — ESP32S3 Dev Module,
ядро Arduino-ESP32 3.x. Нужные библиотеки перечислены в таблице ниже (Менеджер библиотек).

**PlatformIO.** Откройте папку `platformio/` как проект и соберите нужный скетч по имени окружения:

```
pio run -e l1_blink -t upload
pio device monitor
```

Библиотеки PlatformIO скачает сам (`lib_deps` в `platformio.ini`). Свой проект с библиотекой, тестами
и отладкой — `../examples/platformio-lamp` (урок 41).

## Что ещё лежит вне `sketches/`

| Папка | Что это |
|---|---|
| `examples/platformio-lamp` | проект PlatformIO: модуль, тесты на компьютере, отладка (урок 41) |
| `examples/tinyml` | обучение нейросетей на NumPy, экспорт весов (урок 42) |
| `examples/debug_mean` | ESP-IDF и отладка по JTAG (урок 35) |
| `examples/ulp_button` | программа ULP RISC-V, ESP-IDF (урок 36) |
| `examples/web-panels` | точные версии библиотек для ESPUI и WiFiManager (уроки 39–40) |

## Список скетчей

| Урок | Скетч | Библиотеки (Arduino IDE) | Примечание |
|---|---|---|---|
| 05-wifi | `l04_espui_gamma` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 00-intro | `l04_variables` | — |  |
| 01-gpio-output | `l05_blink_millis` | — |  |
| 05-wifi | `l05_espui_hello` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 05-wifi | `l05_espui_led` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 05-wifi | `l05_espui_slider` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 05-wifi | `l05_espui_sta` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 01-gpio-output | `l05_running` | — |  |
| 04-pwm | `l06_breathe_gamma` | — |  |
| 04-pwm | `l06_wave` | — |  |
| 05-wifi | `l07_ble_scan` | — |  |
| 05-wifi | `l07_names` | — |  |
| 05-wifi | `l07_scan` | — |  |
| 44-html | `l08_get_led` | — |  |
| 44-html | `l08_hello_web` | — |  |
| 44-html | `l08_html` | — |  |
| 08-freertos | `l08_panel` | — |  |
| 44-html | `l08_panel_s1` | — |  |
| 44-html | `l08_panel_s2` | — |  |
| 44-html | `l08_post_note` | — |  |
| 02-serial | `l09_guess` | — |  |
| 00-intro | `l0_hello` | — |  |
| 00-basics | `l0_pin_signal` | — |  |
| 00-basics | `l0_setup_loop` | — |  |
| 10-spi | `l10_spi_sd` | — |  |
| 03-gpio-input | `l10_toggle` | — |  |
| 11-storage | `l11_nvs` | — |  |
| 06-adc | `l11_pot` | — |  |
| 06-adc | `l11_pot_servo` | — |  |
| 12-iot | `l12_ble` | — |  |
| 12-iot | `l12_espnow` | — |  |
| 12-iot | `l12_mqtt` | PubSubClient |  |
| 07-i2c | `l12_voltmeter_oled` | Adafruit GFX Library, Adafruit SSD1306 |  |
| 13-security-ota | `l13_https` | — |  |
| 08-freertos | `l13_mutex` | — |  |
| 13-security-ota | `l13_ota` | — |  |
| 14-diagnostics | `l14_panic` | — |  |
| 09-sleep | `l14_sensor_node` | — |  |
| 15-sensors | `l15_dht22` | DHT sensor library (Adafruit) |  |
| 15-sensors | `l15_ds18b20` | OneWire, DallasTemperature |  |
| 15-sensors | `l15_espui_radar` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 15-sensors | `l15_espui_radar_s1` | — |  |
| 15-sensors | `l15_espui_radar_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 15-sensors | `l15_hcsr04` | — |  |
| 16-power | `l16_espui_motor` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 16-power | `l16_espui_motor_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 16-power | `l16_espui_motor_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 16-power | `l16_espui_motor_s3` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 16-power | `l16_mosfet` | — |  |
| 16-power | `l16_motor` | — |  |
| 16-power | `l16_relay` | — |  |
| 17-neopixel | `l17_espui_hue` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 17-neopixel | `l17_espui_hue_s1` | — |  |
| 17-neopixel | `l17_espui_hue_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 17-neopixel | `l17_pixel_builtin` | — |  |
| 17-neopixel | `l17_strip` | Adafruit NeoPixel |  |
| 17-neopixel | `l17_strip_s1` | Adafruit NeoPixel |  |
| 18-http-json | `l18_http_get` | — |  |
| 18-http-json | `l18_json` | ArduinoJson 7 |  |
| 18-http-json | `l18_littlefs` | — |  |
| 18-http-json | `l18_ntp` | — |  |
| 19-usb-hid | `l19_keyboard` | — | в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB) |
| 19-usb-hid | `l19_mouse` | — | в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB) |
| 19-usb-hid | `l19_volume` | — | в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB) |
| 01-gpio-output | `l1_blink` | — |  |
| 01-gpio-output | `l1_blink_millis` | — |  |
| 01-gpio-output | `l1_rgb` | — |  |
| 20-camera | `l20_camera_init` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 20-camera | `l20_camera_init_s1` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 20-camera | `l20_camera_sd` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 20-camera | `l20_camera_sd_s1` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 20-camera | `l20_camera_web` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 20-camera | `l20_camera_web_s1` | — | плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю |
| 21-timers-fsm | `l21_fsm` | — |  |
| 21-timers-fsm | `l21_fsm_s1` | — |  |
| 21-timers-fsm | `l21_fsm_s2` | — |  |
| 21-timers-fsm | `l21_timer` | — |  |
| 22-bits | `l22_bits` | — |  |
| 22-bits | `l22_register` | — |  |
| 22-bits | `l22_shift` | — |  |
| 23-controls | `l23_encoder` | — |  |
| 23-controls | `l23_keypad` | — |  |
| 23-controls | `l23_touch` | — |  |
| 24-sound | `l24_beep` | — |  |
| 24-sound | `l24_espui_notes` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 24-sound | `l24_espui_notes_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 24-sound | `l24_melody` | — |  |
| 25-stepper | `l25_espui_stepper` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 25-stepper | `l25_espui_stepper_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 25-stepper | `l25_espui_stepper_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 25-stepper | `l25_stepper` | — |  |
| 25-stepper | `l25_stepper_s1` | — |  |
| 25-stepper | `l25_stepper_s2` | — |  |
| 26-imu | `l26_espui_tilt` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 26-imu | `l26_espui_tilt_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 26-imu | `l26_espui_tilt_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 26-imu | `l26_mpu6050` | — |  |
| 26-imu | `l26_mpu6050_s1` | — |  |
| 26-imu | `l26_mpu6050_s2` | — |  |
| 27-battery | `l27_battery` | — |  |
| 28-tools | `l28_classes` | — |  |
| 28-tools | `l28_classes_s1` | — |  |
| 28-tools | `l28_classes_s2` | — |  |
| 29-i2s | `l29_i2s_meter` | — |  |
| 02-serial | `l2_bridge` | — |  |
| 30-rmt | `l30_rmt_pulses` | — |  |
| 31-pcnt-mcpwm | `l31_mcpwm_servo` | — |  |
| 31-pcnt-mcpwm | `l31_pcnt` | — |  |
| 32-wired-buses | `l32_rs485` | — |  |
| 32-wired-buses | `l32_twai` | — |  |
| 33-lcd | `l33_tft` | Adafruit GFX Library, Adafruit ST7735 and ST7789 Library |  |
| 34-usb-advanced | `l34_usb_cdc` | — | в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB) |
| 35-idf-jtag | `l35_debug` | — |  |
| 37-edge-ai | `l37_classifier` | — |  |
| 38-reliability | `l38_health` | — |  |
| 39-espui | `l39_espui_hello` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_panel` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_panel_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_panel_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_panel_s3` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_panel_s4` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 06-adc | `l39_espui_voltmeter` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_widgets` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_widgets_s1` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_widgets_s2` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 39-espui | `l39_espui_widgets_s3` | ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson |  |
| 03-gpio-input | `l3_button_read` | — |  |
| 03-gpio-input | `l3_debounce` | — |  |
| 03-gpio-input | `l3_interrupt` | — |  |
| 40-wifi-provisioning | `l40_wifimanager` | WiFiManager 2.0.17 |  |
| 40-wifi-provisioning | `l40_wifimanager_s1` | — |  |
| 40-wifi-provisioning | `l40_wifimanager_s2` | WiFiManager 2.0.17 |  |
| 40-wifi-provisioning | `l40_wifimanager_s3` | WiFiManager 2.0.17 |  |
| 42-tinyml | `l42_imu_collect` | — |  |
| 42-tinyml | `l42_imu_collect_s1` | — |  |
| 42-tinyml | `l42_imu_collect_s2` | — |  |
| 42-tinyml | `l42_imu_collect_s3` | — |  |
| 42-tinyml | `l42_imu_infer` | — |  |
| 42-tinyml | `l42_sine_nn` | — |  |
| 42-tinyml | `l42_sine_nn_s1` | — |  |
| 42-tinyml | `l42_sine_nn_s2` | — |  |
| 04-pwm | `l4_breathe` | — |  |
| 04-pwm | `l4_servo` | — |  |
| 06-adc | `l5_pot` | — |  |
| 06-adc | `l5_voltmeter` | — |  |
| 07-i2c | `l6_oled_hello` | Adafruit GFX Library, Adafruit SSD1306 |  |
| 07-i2c | `l6_scanner` | — |  |
| 08-freertos | `l7_queue` | — |  |
| 08-freertos | `l7_two_tasks` | — |  |
| 05-wifi | `l8_connect` | — |  |
| 44-html | `l8_webserver` | — |  |
| 09-sleep | `l9_timer_sleep` | — |  |

## Фрагменты

Эти примеры — не готовые скетчи (нет `setup()` или это не Arduino-код); их вставляют в скетч по тексту урока:

- `code/l08_panel_css.cpp`
- `code/l20_camera_init_s2.cpp`
- `code/l20_camera_init_s3.cpp`
- `code/l20_camera_sd_s2.cpp`
- `code/l20_camera_web_s2.cpp`
- `code/l20_camera_web_s3.cpp`
- `code/l36_main.cpp` — ESP-IDF, см. examples/ulp_button
- `code/l36_ulp.cpp` — программа ULP RISC-V, см. examples/ulp_button
- `code/l42_imu_infer_s1.cpp`
- `code/l42_imu_infer_s2.cpp`
- `code/l42_imu_infer_s3.cpp`
- `code/l42_imu_infer_s4.cpp`
- `code/l42_sine_nn_s3.cpp`
- `code/l42_sine_nn_s4.cpp`
- `code/l42_sine_nn_s5.cpp`
- `code/l8_softap.cpp`
- `code/l9_ext0.cpp`
