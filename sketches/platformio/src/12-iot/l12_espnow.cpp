#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct __attribute__((packed)) SensorPacket {
  uint32_t sequence;
  float temperature;
};

SensorPacket packet {0, 23.5f};
uint8_t receiver[] = {0x24, 0x6F, 0x28, 0x00, 0x00, 0x00}; // замените MAC адресом приёмника

// В ядре 3.3 изменился первый аргумент колбэка отправки
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
void onSent(const esp_now_send_info_t*, esp_now_send_status_t status) {
#else
void onSent(const uint8_t*, esp_now_send_status_t status) {
#endif
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "приёмник подтвердил (ACK)" : "нет ACK");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) { Serial.println("ESP-NOW не запустился"); return; }
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer{};
  memcpy(peer.peer_addr, receiver, 6);
  peer.channel = 0;                 // текущий канал; у обеих плат он должен совпадать
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void loop() {
  packet.sequence++;
  esp_now_send(receiver, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
  delay(1000);
}
