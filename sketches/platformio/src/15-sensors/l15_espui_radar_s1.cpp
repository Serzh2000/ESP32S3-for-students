#include <Arduino.h>
// Шаг 1. Только датчик: расстояние HC-SR04 печатаем в Serial Monitor (TRIG -> GPIO16, ECHO -> GPIO21 через делитель).
const int TRIG_PIN = 16;
const int ECHO_PIN = 21;
const uint32_t ECHO_TIMEOUT_US = 30000;

float readDistanceCm() {                  // тот же код, что в листинге урока
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  uint32_t us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (us == 0) return -1;                 // эхо не вернулось
  return us * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 200) {           // не чаще 5 замеров в секунду
    last = millis();
    float d = readDistanceCm();
    if (d < 0) Serial.println("нет эха");
    else Serial.println(d);
  }
}
