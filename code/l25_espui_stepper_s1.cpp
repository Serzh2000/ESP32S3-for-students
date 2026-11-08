// Шаг 1 (урок 25): панель считает, на сколько градусов нужно повернуть. Мотор пока не подключён.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

std::atomic<long> requestDeg{0};          // накопленная просьба: «повернись ещё на столько»
std::atomic<int> stepSize{90};
long targetDeg = 0;                       // цель в градусах: меняет только loop()
uint16_t angleId;

void stepCallback(Control* sender, int event) {
  if (event == N_VALUE) stepSize = constrain(sender->value.toInt(), 1, 360);
}
void plusCallback(Control*, int event) { if (event == B_UP) requestDeg += stepSize; }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Stepper", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  ESPUI.number("Шаг поворота, °", stepCallback, ControlColor::Peterriver, 90, 1, 360);
  ESPUI.button("Вправо", plusCallback, ControlColor::Emerald, "+ шаг");
  angleId = ESPUI.label("Цель, °", ControlColor::Turquoise, "0");
  ESPUI.begin("ESP32-S3 stepper");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  long deg = requestDeg.exchange(0);      // забираем накопленные просьбы одним действием
  if (deg != 0) {
    targetDeg += deg;
    ESPUI.updateLabel(angleId, String(targetDeg));
  }
}
