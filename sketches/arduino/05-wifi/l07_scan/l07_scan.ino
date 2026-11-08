// Какие Wi-Fi сети видит плата?
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);                       // режим «клиент»
  Serial.print("MAC-адрес платы: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Serial.println("Ищу сети...");
  int n = WiFi.scanNetworks();               // занимает пару секунд
  for (int i = 0; i < n; i++) {
    Serial.printf("%2d) %-20s канал %2d, сигнал %4d дБм, %s\n",
                  i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i),
                  WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "открытая" : "с паролем");
  }
  WiFi.scanDelete();                         // освободить память
  delay(10000);
}
