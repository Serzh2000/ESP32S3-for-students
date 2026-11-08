// Шаг 3. Крестовина Pad: события приходят парами «нажали / отпустили».
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
void padCallback(Control*, int event) {
  switch (event) {
    case P_FOR_DOWN:    Serial.println("вперёд: нажата");   break;
    case P_FOR_UP:      Serial.println("вперёд: отпущена"); break;
    case P_BACK_DOWN:   Serial.println("назад: нажата");    break;
    case P_LEFT_DOWN:   Serial.println("влево: нажата");    break;
    case P_RIGHT_DOWN:  Serial.println("вправо: нажата");   break;
    case P_CENTER_DOWN: Serial.println("центр: нажата");    break;
  }
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
  ESPUI.addControl(ControlType::PadWithCenter, "Pad", "", ControlColor::Carrot, inputs, padCallback);
  ESPUI.begin("ESP32-S3 widgets");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  delay(100);
}
