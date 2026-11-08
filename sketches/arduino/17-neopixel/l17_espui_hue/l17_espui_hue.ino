// Шаг 3 (итог). Второй ползунок задаёт яркость, подпись показывает получившийся RGB.
// Встроенный RGB-светодиод (GPIO48, на ревизии v1.1 -- GPIO38): оттенок и яркость с телефона.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int RGB_PIN = 48;
std::atomic<int> hueValue{0};             // 0..359
std::atomic<int> brightness{40};          // 0..255, для глаз берём небольшую
uint16_t colorId;

void hsvToRgb(int hue, uint8_t sat, uint8_t val, uint8_t &r, uint8_t &g, uint8_t &b) {
  int region = hue / 60;                  // тот же перевод HSV -> RGB, что в уроке
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
void brightCallback(Control* sender, int event) {
  if (event == SL_VALUE) brightness = constrain(sender->value.toInt(), 0, 255);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Hue", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  ESPUI.sliderContinuous = true;          // цвет должен меняться, пока двигаем ползунок
  ESPUI.slider("Hue, °", hueCallback, ControlColor::Carrot, 0, 0, 359);
  ESPUI.slider("Яркость (0..255)", brightCallback, ControlColor::Wetasphalt, 40, 0, 255);
  colorId = ESPUI.label("Цвет RGB", ControlColor::Turquoise, "0, 0, 0");
  ESPUI.begin("ESP32-S3 hue");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  uint8_t r, g, b;
  hsvToRgb(hueValue, 255, brightness, r, g, b);
  rgbLedWrite(RGB_PIN, r, g, b);

  static uint32_t last = 0;
  if (millis() - last >= 300) {           // подпись обновляем редко, цвет -- каждый проход
    last = millis();
    ESPUI.updateLabel(colorId, String(r) + ", " + String(g) + ", " + String(b));
  }
  delay(10);
}
