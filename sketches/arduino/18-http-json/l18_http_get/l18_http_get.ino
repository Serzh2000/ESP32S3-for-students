#include <WiFi.h>
#include <HTTPClient.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
// Замените адрес на свой: это условный пример, такого сервиса может не быть.
const char* URL = "http://example.com/api/weather.json";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }

  HTTPClient http;
  http.setTimeout(8000);                  // не ждать ответ дольше 8 с
  http.begin(URL);
  int code = http.GET();                  // 200 -- успех, отрицательное число -- ошибка сети
  if (code == 200) {
    String body = http.getString();
    Serial.println(body);
  } else {
    Serial.printf("Ошибка запроса: %d (%s)\n", code, HTTPClient::errorToString(code).c_str());
  }
  http.end();                             // освободить соединение
}

void loop() {}
