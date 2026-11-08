#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
// Замените адрес на свой. Ожидаемый ответ: {"city":"Moscow","temp":-3.5,"humidity":80}
const char* URL = "http://example.com/api/weather.json";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }

  HTTPClient http;
  http.setTimeout(8000);
  http.begin(URL);
  if (http.GET() == 200) {
    JsonDocument doc;                                      // ArduinoJson 7
    DeserializationError err = deserializeJson(doc, http.getString());
    if (err) {                                             // ответ не похож на JSON
      Serial.printf("Ошибка разбора: %s\n", err.c_str());
    } else {
      const char* city = doc["city"] | "?";                // "?" -- значение по умолчанию
      float temp = doc["temp"] | 0.0f;
      int humidity = doc["humidity"] | -1;
      Serial.printf("%s: %.1f °C, влажность %d %%\n", city, temp, humidity);
    }
  } else {
    Serial.println("Сервер не ответил кодом 200");
  }
  http.end();
}

void loop() {}
