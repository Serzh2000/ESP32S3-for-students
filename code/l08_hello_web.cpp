// Самый простой веб-сервер: страница «Привет!»
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

WebServer server(80);                        // 80 -- стандартный «порт» для HTTP

void handleRoot() {                          // кто-то открыл адрес "/"
  String html = "<h1>Привет! Я ESP32-S3</h1>";
  html += "<p>Я работаю уже " + String(millis() / 1000) + " секунд.</p>";
  server.send(200, "text/html; charset=utf-8", html);  // 200 = «всё хорошо»
}

void handleNotFound() {                      // адрес, которого у нас нет
  server.send(404, "text/plain; charset=utf-8", "Такой страницы нет");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.printf("\nОткрой http://%s или http://esp32.local\n",
                WiFi.localIP().toString().c_str());
  MDNS.begin("esp32");

  server.on("/", handleRoot);                // адрес -> функция-обработчик
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();                     // ответить браузеру, если он что-то спросил
}
