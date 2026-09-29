// Подключаемся к домашней сети как клиент (станция)
#include <WiFi.h>

const char* SSID = "MyNetwork";              // имя твоей сети
const char* PASS = "MyPassword";             // пароль

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.print("Подключаюсь");
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nНе получилось. Проверь имя и пароль сети.");
    return;
  }
  Serial.println("\nГотово!");
  Serial.printf("MAC-адрес: %s\n", WiFi.macAddress().c_str());
  Serial.printf("IP-адрес:  %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Маска:     %s\n", WiFi.subnetMask().toString().c_str());
  Serial.printf("Роутер:    %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.printf("Сигнал:    %d дБм\n", WiFi.RSSI());
}

void loop() {}
