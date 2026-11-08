#include <Arduino.h>
// Шаг 2. Переключатель в панели включает светодиод на GPIO4 (резистор 220 Ом к земле).
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int LED_PIN = 4;
std::atomic<bool> ledOn{false};           // сюда пишет панель, читает loop()

void switchCallback(Control* sender, int event) {   // вызывается при нажатии в браузере
  if (event == S_ACTIVE)   ledOn = true;
  if (event == S_INACTIVE) ledOn = false;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-05-demo");

  ESPUI.switcher("Светодиод", switchCallback, ControlColor::Emerald, false);
  ESPUI.begin("Светодиод");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);   // выводом управляет только loop()
  delay(5);
}
