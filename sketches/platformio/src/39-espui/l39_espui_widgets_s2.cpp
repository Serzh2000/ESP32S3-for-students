#include <Arduino.h>
// Шаг 2. К числу и тексту добавляется список выбора (Select с вариантами Option).
#include <WiFi.h>
#include <ESPUI.h>

void numberCallback(Control* sender, int event) {
  if (event != N_VALUE) return;
  Serial.printf("Number = %d\n", sender->value.toInt());
}
void textCallback(Control* sender, int event) {
  if (event != T_VALUE) return;
  Serial.printf("Text = '%s'\n", sender->value.c_str());
}
void selectCallback(Control* sender, int event) {
  if (event != S_VALUE) return;
  Serial.printf("Select = %s\n", sender->value.c_str());
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Widgets", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  auto inputs = ESPUI.addControl(ControlType::Tab, "", "Input");
  auto numberId = ESPUI.addControl(ControlType::Number, "Number 0..50", "10", ControlColor::Peterriver, inputs, numberCallback);
  ESPUI.addControl(ControlType::Min, "", "0", ControlColor::None, numberId);
  ESPUI.addControl(ControlType::Max, "", "50", ControlColor::None, numberId);
  ESPUI.addControl(ControlType::Text, "Text", "hello", ControlColor::Emerald, inputs, textCallback);
  auto selectId = ESPUI.addControl(ControlType::Select, "Select", "do", ControlColor::Sunflower, inputs, selectCallback);
  ESPUI.addControl(ControlType::Option, "do (C4)", "do", ControlColor::None, selectId);
  ESPUI.addControl(ControlType::Option, "re (D4)", "re", ControlColor::None, selectId);
  ESPUI.addControl(ControlType::Option, "mi (E4)", "mi", ControlColor::None, selectId);
  ESPUI.begin("ESP32-S3 widgets");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  delay(100);
}
