#include <Arduino.h>
const int TRIG_PIN = 16;                  // выход: запускающий импульс
const int ECHO_PIN = 21;                  // вход: ECHO через делитель 5 В -> 3,3 В
const uint32_t ECHO_TIMEOUT_US = 30000;   // ~5 м туда и обратно; дольше не ждём

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);           // импульс не короче 10 мкс запускает измерение
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  uint32_t us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);  // длительность эха
  if (us == 0) return -1;                 // тайм-аут: эхо не вернулось
  return us * 0.0343f / 2.0f;             // скорость звука 343 м/с = 0,0343 см/мкс
}

void loop() {
  float d = readDistanceCm();
  if (d < 0) Serial.println("Нет эха: объект далеко или нет питания");
  else       Serial.printf("Расстояние: %.1f см\n", d);
  delay(100);                             // пауза между замерами не меньше 60 мс
}
