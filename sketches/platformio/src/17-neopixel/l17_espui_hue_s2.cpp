#include <Arduino.h>
// Шаг 2. Оттенок теперь выбирает ползунок в панели ESPUI. Яркость пока фиксированная.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int RGB_PIN = 48;
std::atomic<int> hueValue{0};             // 0..359, пишет обработчик панели

void hsvToRgb(int hue, uint8_t sat, uint8_t val, uint8_t &r, uint8_t &g, uint8_t &b) {
  int region = hue / 60;
  int rem = (hue % 60) * 255 / 60;
  uint8_t p = val * (255 - sat) / 255;
  uint8_t q = val * (255 - sat * rem / 255) / 255;
  uint8_t t = val * (255 - sat * (255 - rem) / 255) / 255;
  switch (region) {
    case 0:  r = val; g = t;   b = p;   break;
    case 1:  r = q;   g = val; b = p;   break;
    case 2:  r = p;   g = val; b = t;   break;
    case 3:  r = p;   g = q;   b = val; break;
    case 4:  r = t;   g = p;   b = val; break;
    default: r = val; g = p;   b = q;   break;
  }
}

void hueCallback(Control* sender, int event) {
  if (event == SL_VALUE) hueValue = constrain(sender->value.toInt(), 0, 359);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Hue", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  ESPUI.sliderContinuous = true;          // цвет должен меняться, пока двигаем ползунок
  ESPUI.slider("Hue, °", hueCallback, ControlColor::Carrot, 0, 0, 359);
  ESPUI.begin("ESP32-S3 hue");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  uint8_t r, g, b;
  hsvToRgb(hueValue, 255, 40, r, g, b);
  rgbLedWrite(RGB_PIN, r, g, b);          // светодиодом управляет только loop()
  delay(10);
}
