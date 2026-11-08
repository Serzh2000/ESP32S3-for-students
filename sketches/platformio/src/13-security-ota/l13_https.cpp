#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

const char* rootCA = R"EOF(
-----BEGIN CERTIFICATE-----
... корневой сертификат центра сертификации вашего сервера ...
-----END CERTIFICATE-----
)EOF";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 10000)) {
    Serial.println("Нет точного времени: TLS-проверка сертификата невозможна");
    return;
  }

  WiFiClientSecure client;
  client.setCACert(rootCA);             // не используйте setInsecure() в готовом устройстве
  if (!client.connect("example.com", 443)) {
    Serial.println("TLS-соединение не установлено");
    return;
  }
  client.print("GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n");
  uint32_t t0 = millis();
  while ((client.connected() || client.available()) && millis() - t0 < 10000) {
    if (client.available()) Serial.write(client.read());  // печатаем ответ сервера
  }
  client.stop();
}

void loop() {}
