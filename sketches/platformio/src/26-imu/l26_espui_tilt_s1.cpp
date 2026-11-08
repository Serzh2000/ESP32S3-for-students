#include <Arduino.h>
// Шаг 1 (урок 26): панель с графиком. Пока рисуем «синусоиду» вместо угла -- датчик не нужен.
#include <WiFi.h>
#include <ESPUI.h>

uint16_t graphId, valueId;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Tilt", "lesson-39-demo")) { Serial.println("AP failed"); return; }
  valueId = ESPUI.label("Значение, °", ControlColor::Emerald, "0");
  graphId = ESPUI.graph("График", ControlColor::Turquoise);
  ESPUI.begin("ESP32-S3 tilt");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t lastShow = 0;
  if (millis() - lastShow >= 250) {       // показываем четыре раза в секунду
    lastShow = millis();
    float fake = 30 * sin(millis() / 1000.0f);          // пока вместо угла
    ESPUI.updateLabel(valueId, String(fake, 1));
    ESPUI.addGraphPoint(graphId, (int)fake);
  }
  delay(10);
}
