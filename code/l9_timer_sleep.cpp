#include "esp_sleep.h"

RTC_DATA_ATTR int bootCount = 0;          // переживает deep-sleep

const uint64_t SLEEP_US = 10ULL * 1000000; // 10 секунд

void setup() {
  Serial.begin(115200);
  delay(500);
  bootCount++;
  Serial.printf("Пробуждение №%d, причина: %d\n",
                bootCount, esp_sleep_get_wakeup_cause());

  // ... здесь измерения и отправка данных ...

  esp_sleep_enable_timer_wakeup(SLEEP_US);
  Serial.println("Засыпаю");
  Serial.flush();
  esp_deep_sleep_start();                  // сюда программа уже не вернётся
}

void loop() {}                             // не вызывается
