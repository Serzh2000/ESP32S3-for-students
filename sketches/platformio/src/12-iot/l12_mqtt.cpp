#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID   = "MyNetwork";
const char* PASS   = "MyPassword";
const char* BROKER = "192.168.1.10";            // адрес вашего брокера

WiFiClient net;
PubSubClient mqtt(net);
uint32_t lastTry = 0, lastPub = 0;

void onMessage(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];
  Serial.printf("%s -> %s\n", topic, msg.c_str());  // здесь проверяем и применяем команду
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);                       // подключение идёт в фоне
  mqtt.setServer(BROKER, 1883);
  mqtt.setCallback(onMessage);
}

void loop() {
  if (!mqtt.connected()) {
    // одна попытка раз в 5 секунд, а не бесконечный цикл ожидания
    if (WiFi.status() == WL_CONNECTED && millis() - lastTry > 5000) {
      lastTry = millis();
      if (mqtt.connect("esp-01")) mqtt.subscribe("home/esp-01/set");
    }
    return;
  }
  mqtt.loop();                                  // приём сообщений и keep-alive
  if (millis() - lastPub > 10000) {
    lastPub = millis();
    String state = String("{\"uptime\":") + millis() / 1000 + "}";
    mqtt.publish("home/esp-01/state", state.c_str(), true);  // true = retained
  }
}
