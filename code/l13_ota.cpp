#include <WiFi.h>
#include <ArduinoOTA.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("esp32s3-lab");
  ArduinoOTA.setPassword("change-me");  // без пароля прошить плату может любой в сети
  ArduinoOTA.onStart([] { Serial.println("Начато OTA-обновление"); });
  ArduinoOTA.onError([](ota_error_t e) { Serial.printf("OTA ошибка: %u\n", e); });
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();  // вызывается часто; не блокируйте loop() длинными delay()
}
