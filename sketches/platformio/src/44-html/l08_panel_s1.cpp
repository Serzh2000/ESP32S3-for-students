#include <Arduino.h>
// Панель, шаг 1: только «серверная» часть. Откройте /api в браузере -- увидите состояние в формате JSON.
#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 4;
WebServer server(80);
bool ledOn = false;
int level = 50;                              // яркость, %

void applyLed() { ledcWrite(LED_PIN, ledOn ? level * 255 / 100 : 0); }

void handleApi() {
  if (server.method() == HTTP_POST) {        // POST -- команда изменить состояние
    if (server.hasArg("toggle")) ledOn = !ledOn;
    if (server.hasArg("level")) {
      level = constrain(server.arg("level").toInt(), 0, 100);
      ledOn = true;
    }
    applyLed();
  }
  // и на GET, и на POST отвечаем текущим состоянием в формате JSON
  String json = String("{\"on\":") + (ledOn ? "true" : "false") +
                ",\"level\":" + level + "}";
  server.send(200, "application/json", json);
}

void setup() {
  ledcAttach(LED_PIN, 5000, 8);
  WiFi.softAP("S3-Web", "lesson-44-demo");
  server.on("/api", handleApi);
  server.begin();
}

void loop() {
  server.handleClient();
}
