#include <Arduino.h>
// LED: GPIO4 -> 220 Ом -> LED -> GND. Панель: ползунок яркости и переключатель гаммы.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int LED_PIN = 4;
std::atomic<int> wantLevel{0};            // сюда пишет callback, читает loop()
std::atomic<bool> gammaOn{true};
uint16_t dutyId;

uint32_t gammaDuty(int percent) {         // как в уроке 4: 0..100 % -> 0..255
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void levelCallback(Control* sender, int event) {
  if (event != SL_VALUE) return;
  wantLevel = constrain(sender->value.toInt(), 0, 100);   // ползунок и так 0..100, но проверяем
}

void gammaCallback(Control*, int event) {
  if (event == S_ACTIVE) gammaOn = true;
  if (event == S_INACTIVE) gammaOn = false;
}

void setup() {
  Serial.begin(115200);
  if (!ledcAttach(LED_PIN, 5000, 8)) { Serial.println("PWM init failed"); return; }
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Gamma", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  ESPUI.sliderContinuous = false;
  auto level = ESPUI.slider("Яркость для глаза, %", levelCallback, ControlColor::Peterriver, 0, 0, 100);
  ESPUI.switcher("Гамма-коррекция", gammaCallback, ControlColor::Emerald, true);
  dutyId = ESPUI.label("Коэффициент заполнения (0..255)", ControlColor::Turquoise, "0");
  (void)level;
  ESPUI.begin("ESP32-S3 gamma");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  int percent = wantLevel;
  uint32_t duty = gammaOn ? gammaDuty(percent) : percent * 255 / 100;
  ledcWrite(LED_PIN, duty);               // аппаратную часть трогает только loop()

  static uint32_t last = 0;
  if (millis() - last >= 300) {           // показываем применённое значение
    last = millis();
    ESPUI.updateLabel(dutyId, String(duty));
  }
  delay(5);
}
