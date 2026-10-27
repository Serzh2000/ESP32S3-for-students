// Потенциометр: крайние выводы к 3V3 и GND, движок -> GPIO1 (ADC1).
#include <WiFi.h>
#include <ESPUI.h>

const int POT_PIN = 1;
uint16_t gaugeId, graphId, valueId;

int readMilliVolts() {                              // среднее из 16 замеров гасит шум
  uint32_t sum = 0;
  for (int i = 0; i < 16; i++) sum += analogReadMilliVolts(POT_PIN);
  return sum / 16;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Voltmeter", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  gaugeId = ESPUI.addControl(ControlType::Gauge, "Напряжение, мВ", "0", ControlColor::Wetasphalt);
  ESPUI.addControl(ControlType::Min, "", "0", ControlColor::None, gaugeId);
  ESPUI.addControl(ControlType::Max, "", "3300", ControlColor::None, gaugeId);
  valueId = ESPUI.label("Значение", ControlColor::Peterriver, "0 мВ");
  graphId = ESPUI.graph("Последние отсчёты", ControlColor::Emerald);
  ESPUI.begin("ESP32-S3 voltmeter");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 500) {                     // два обновления в секунду достаточно глазу
    last = millis();
    int mv = readMilliVolts();
    ESPUI.updateControlValue(gaugeId, String(mv));
    ESPUI.updateLabel(valueId, String(mv) + " мВ");
    ESPUI.addGraphPoint(graphId, mv);
  }
  delay(5);
}
