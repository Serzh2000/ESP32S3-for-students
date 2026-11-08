#include <Arduino.h>
// Шаг 3. К переключателю добавляется ползунок яркости (ШИМ из урока 4) и подпись со значением.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int LED_PIN = 4;
std::atomic<bool> ledOn{false};
std::atomic<int> level{50};               // яркость 0..100 %
uint16_t levelLabel;

void switchCallback(Control* sender, int event) {
  if (event == S_ACTIVE)   ledOn = true;
  if (event == S_INACTIVE) ledOn = false;
}

void sliderCallback(Control* sender, int event) {
  level = constrain(sender->value.toInt(), 0, 100);
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);           // ШИМ 5 кГц, 8 бит: значения 0..255
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-05-demo");

  ESPUI.sliderContinuous = false;         // отправлять значение, когда палец отпущен
  ESPUI.switcher("Светодиод", switchCallback, ControlColor::Emerald, false);
  ESPUI.slider("Яркость, %", sliderCallback, ControlColor::Peterriver, 50, 0, 100);
  levelLabel = ESPUI.label("Сейчас на выводе", ControlColor::Turquoise, "0");
  ESPUI.begin("Лампа");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  int duty = ledOn ? level * 255 / 100 : 0;
  ledcWrite(LED_PIN, duty);
  ESPUI.updateLabel(levelLabel, String(duty));
  delay(200);
}
