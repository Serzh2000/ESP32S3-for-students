#include <Arduino.h>
const int RELAY_PIN = 15;                 // вход IN модуля реле
const bool ACTIVE_LOW = true;             // у большинства модулей включает уровень LOW

void relay(bool on) {
  digitalWrite(RELAY_PIN, (on == ACTIVE_LOW) ? LOW : HIGH);
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  relay(false);                           // сразу выключить: для модуля LOW -- это «включено»
}

void loop() {
  relay(true);                            // щелчок: контакты замкнулись
  delay(2000);
  relay(false);
  delay(3000);                            // реле не переключают чаще раза в секунду
}
