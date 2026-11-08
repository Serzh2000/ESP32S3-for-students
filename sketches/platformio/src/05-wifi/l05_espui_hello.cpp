#include <Arduino.h>
// Шаг 1. Первая панель: платформа сама создаёт сеть, в панели одна строка с временем работы.
#include <WiFi.h>
#include <ESPUI.h>

uint16_t uptimeId;                        // номер элемента, чтобы потом менять его текст

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);                     // плата сама раздаёт сеть
  WiFi.softAP("S3-Panel", "lesson-05-demo");

  ESPUI.label("Плата", ControlColor::Peterriver, "Привет из ESP32-S3!");
  uptimeId = ESPUI.label("Работает, с", ControlColor::Emerald, "0");
  ESPUI.begin("Моя первая панель");       // запустить веб-сервер панели
  Serial.println(WiFi.softAPIP());        // этот адрес вводим в браузер телефона
}

void loop() {
  ESPUI.updateLabel(uptimeId, String(millis() / 1000));
  delay(1000);
}
