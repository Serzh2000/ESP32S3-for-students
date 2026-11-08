// Шаг 3 (итог). К шкале добавляются подпись состояния и график; ошибка датчика не похожа на «0 см».
// HC-SR04: TRIG -> GPIO16, ECHO -> GPIO21 через делитель 1 кОм / 2 кОм (урок 15).
#include <WiFi.h>
#include <ESPUI.h>

const int TRIG_PIN = 16;
const int ECHO_PIN = 21;
const uint32_t ECHO_TIMEOUT_US = 30000;
uint16_t gaugeId, graphId, stateId;

float readDistanceCm() {                  // тот же код, что в листинге урока
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
  stateId = ESPUI.label("Состояние", ControlColor::Peterriver, "ждём измерение");
  graphId = ESPUI.graph("История расстояния", ControlColor::Emerald);
  ESPUI.begin("ESP32-S3 radar");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 200) {           // не чаще 5 замеров в секунду
    last = millis();
    float d = readDistanceCm();
    if (d < 0) {
      ESPUI.updateLabel(stateId, "нет эха: далеко или нет питания");   // ошибка, а не ложный ноль
    } else {
      int cm = constrain((int)d, 0, 200);
      ESPUI.updateControlValue(gaugeId, String(cm));
      ESPUI.updateLabel(stateId, String(d, 1) + " см");
      ESPUI.addGraphPoint(graphId, cm);
    }
  }
  delay(5);
}
