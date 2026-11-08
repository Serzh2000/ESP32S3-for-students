// Шаг 4. Та же лампа, но плата входит в вашу Wi-Fi-сеть; не получилось — раздаёт свою.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const char* SSID = "ИМЯ_СЕТИ";            // впишите свою сеть 2.4 ГГц
const char* PASS = "ПАРОЛЬ";
const int LED_PIN = 4;

std::atomic<bool> ledOn{false};
std::atomic<int> level{50};
uint16_t signalLabel;

void switchCallback(Control* sender, int event) {
  ledOn = (event == S_ACTIVE);
}

void sliderCallback(Control* sender, int event) {
  level = constrain(sender->value.toInt(), 0, 100);
}

bool connectToRouter() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(250);
  return WiFi.status() == WL_CONNECTED;
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);

  if (connectToRouter()) {
    Serial.println(WiFi.localIP());       // адрес для браузера в той же сети
  } else {
    WiFi.mode(WIFI_AP);                   // запасной вариант: своя сеть
    WiFi.softAP("S3-Panel", "lesson-05-demo");
    Serial.println(WiFi.softAPIP());
  }

  ESPUI.sliderContinuous = false;
  ESPUI.switcher("Светодиод", switchCallback, ControlColor::Emerald, false);
  ESPUI.slider("Яркость, %", sliderCallback, ControlColor::Peterriver, 50, 0, 100);
  signalLabel = ESPUI.label("Уровень сигнала, дБм", ControlColor::Turquoise, "—");
  ESPUI.begin("Лампа");
}

void loop() {
  ledcWrite(LED_PIN, ledOn ? level * 255 / 100 : 0);
  if (WiFi.getMode() == WIFI_STA) ESPUI.updateLabel(signalLabel, String(WiFi.RSSI()));
  delay(200);
}
