// Постоянный IP-адрес, свой MAC-адрес и имя esp32-led.local
#include <WiFi.h>
#include <ESPmDNS.h>
#include <esp_wifi.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

IPAddress ip(192, 168, 1, 50);               // адрес, который мы хотим получить
IPAddress gateway(192, 168, 1, 1);           // адрес роутера
IPAddress subnet(255, 255, 255, 0);          // маска: «своя» сеть -- 192.168.1.*
IPAddress dns(192, 168, 1, 1);               // кто переводит имена сайтов в адреса

uint8_t myMac[6] = {0x02, 0x12, 0x34, 0x56, 0x78, 0x9A};  // «самодельный» MAC

void setup() {
  Serial.begin(115200);
  WiFi.setHostname("esp32-led");             // имя в списке устройств роутера
  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(WIFI_IF_STA, myMac);      // по желанию: сменить MAC
  WiFi.config(ip, gateway, subnet, dns);     // постоянный IP вместо выданного роутером
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\nMAC: %s, IP: %s\n",
                WiFi.macAddress().c_str(), WiFi.localIP().toString().c_str());

  if (MDNS.begin("esp32-led")) {             // включить имя esp32-led.local
    Serial.println("Плата отзывается на имя esp32-led.local");
  }
}

void loop() {}
