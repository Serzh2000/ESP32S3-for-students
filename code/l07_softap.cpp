// Плата сама создаёт Wi-Fi сеть (точка доступа)
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);                        // режим «точка доступа»
  WiFi.softAP("ESP32-S3", "12345678");       // имя сети и пароль (не короче 8 символов)
  Serial.print("Сеть создана. Адрес платы: ");
  Serial.println(WiFi.softAPIP());           // обычно 192.168.4.1
  Serial.printf("MAC точки доступа: %s\n", WiFi.softAPmacAddress().c_str());
}

void loop() {
  static int last = -1;
  int n = WiFi.softAPgetStationNum();        // сколько устройств подключилось
  if (n != last) {
    last = n;
    Serial.printf("Подключено устройств: %d\n", n);
  }
  delay(500);
}
