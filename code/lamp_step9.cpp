// Умная лампа, шаг 9: сон, когда лампа выключена
#include "driver/rtc_io.h"

const uint32_t SLEEP_AFTER_MS = 60000;       // минута без дела -> сон
volatile uint32_t lastActivity = 0;

RTC_DATA_ATTR LampState lamp = {false, 50};  // было: LampState lamp -- теперь переживает сон

void applyLamp() {
  ledcWrite(LAMP_PIN, lamp.on ? levelToDuty(lamp.level) : 0);
  lastActivity = millis();                   // любое изменение = активность
}

void goToSleep() {
  vTaskSuspend(displayHandle);               // экран больше не рисуется
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  rgbLedWrite(RGB_PIN, 0, 0, 0);
  rtc_gpio_pullup_en((gpio_num_t)BTN_PIN);   // подтяжка, работающая во сне
  rtc_gpio_pulldown_dis((gpio_num_t)BTN_PIN);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_PIN, 0);
  Serial.println("Сплю. Разбудит кнопка.");
  Serial.flush();
  esp_deep_sleep_start();
}

void setup() {
  rtc_gpio_deinit((gpio_num_t)BTN_PIN);      // вернуть кнопку в обычный режим GPIO
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
    lamp.on = true;                          // разбудили кнопкой -> включаем
  // ... дальше без изменений (шаг 8)
}

void loop() {
  handleSerial();
  server.handleClient();
  heartbeat();
  if (!getLamp().on && millis() - lastActivity > SLEEP_AFTER_MS) goToSleep();
  delay(2);
}
