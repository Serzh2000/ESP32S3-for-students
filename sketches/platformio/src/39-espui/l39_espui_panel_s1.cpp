#include <Arduino.h>
// Шаг 1. Вкладка и переключатель LED. GPIO4 -> резистор 220 Ом -> LED -> GND.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

std::atomic<bool> wantOn{false};          // желание пользователя; железом управляет loop()

void powerCallback(Control*, int event) {
  if (event == S_ACTIVE)   wantOn = true;
  if (event == S_INACTIVE) wantOn = false;
}

void setup() {
  Serial.begin(115200);
  ledcAttach(4, 5000, 8);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-39-demo");
  auto mainTab = ESPUI.addControl(ControlType::Tab, "", "Control");
  ESPUI.addControl(ControlType::Switcher, "LED enabled", "0",
      ControlColor::Emerald, mainTab, powerCallback);
  ESPUI.begin("ESP32-S3 control panel");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  ledcWrite(4, wantOn ? 255 : 0);
  delay(10);
}
