#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

WebServer server(80);

void handleRoot() {                       // пока просто страница с заголовком
  server.send(200, "text/html",
              "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body>"
              "<h3>ESP32-S3 CAM</h3></body></html>");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }
  Serial.print("Откройте в браузере: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
