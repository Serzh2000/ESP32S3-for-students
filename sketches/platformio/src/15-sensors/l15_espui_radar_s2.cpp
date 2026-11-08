#include <Arduino.h>
// Шаг 2. К датчику добавляется панель ESPUI: круговая шкала (Gauge) показывает расстояние.
#include <WiFi.h>
#include <ESPUI.h>

const int TRIG_PIN = 16;
const int ECHO_PIN = 21;
const uint32_t ECHO_TIMEOUT_US = 30000;
uint16_t gaugeId;

float readDistanceCm() {                  // без изменений
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  uint32_t us = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (us == 0) return -1;
  return us * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Radar", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  gaugeId = ESPUI.addControl(ControlType::Gauge, "Расстояние, см", "0", ControlColor::Wetasphalt);
  ESPUI.addControl(ControlType::Min, "", "0", ControlColor::None, gaugeId);
  ESPUI.addControl(ControlType::Max, "", "200", ControlColor::None, gaugeId);
  ESPUI.begin("ESP32-S3 radar");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 200) {
    last = millis();
    float d = readDistanceCm();
    if (d >= 0) {                         // пока просто пропускаем неудачные замеры
      ESPUI.updateControlValue(gaugeId, String(constrain((int)d, 0, 200)));
    }
  }
  delay(5);
}
