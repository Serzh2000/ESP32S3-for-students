// «Датчик на батарейке»: просыпается раз в 30 с или от кнопки,
// измеряет напряжение, мигает светодиодом и снова засыпает
#include "driver/rtc_io.h"

const int LED_PIN = 4;
const int BTN_PIN = 5;
const int POT_PIN = 1;                       // «датчик» -- потенциометр
const uint64_t SLEEP_US = 30ULL * 1000000;   // 30 секунд

RTC_DATA_ATTR int wakeCount = 0;             // эти переменные
RTC_DATA_ATTR int buttonCount = 0;           // переживают сон

void setup() {
  Serial.begin(115200);
  delay(300);
  rtc_gpio_deinit((gpio_num_t)BTN_PIN);      // вернуть кнопку в обычный режим
  pinMode(LED_PIN, OUTPUT);
  wakeCount++;

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    buttonCount++;                           // разбудили кнопкой
    Serial.println("Меня разбудила кнопка!");
  }
  int mv = analogReadMilliVolts(POT_PIN);
  Serial.printf("Пробуждение %d (кнопкой: %d), U = %d мВ\n", wakeCount, buttonCount, mv);

  digitalWrite(LED_PIN, HIGH);               // короткая вспышка: «я работаю»
  delay(100);
  digitalWrite(LED_PIN, LOW);

  esp_sleep_enable_timer_wakeup(SLEEP_US);   // будильник 1: таймер
  rtc_gpio_pullup_en((gpio_num_t)BTN_PIN);   // будильник 2: кнопка
  rtc_gpio_pulldown_dis((gpio_num_t)BTN_PIN);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_PIN, 0);
  Serial.println("Сплю...");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {}                               // сюда программа не доходит
