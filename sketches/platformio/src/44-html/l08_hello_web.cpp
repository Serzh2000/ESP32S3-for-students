#include <Arduino.h>
// Самый простой веб-сервер: страница «Привет!». Плата сама раздаёт сеть, роутер не нужен.
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);                        // 80 -- стандартный «порт» для HTTP

void handleRoot() {                          // кто-то открыл адрес "/"
  String html = "<h1>Привет! Я ESP32-S3</h1>";
  html += "<p>Я работаю уже " + String(millis() / 1000) + " секунд.</p>";
  server.send(200, "text/html; charset=utf-8", html);  // 200 = «всё хорошо»
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP("S3-Web", "lesson-44-demo");   // своя сеть, как в уроке 5
  Serial.println(WiFi.softAPIP());           // этот адрес вводим в браузере
  server.on("/", handleRoot);                // адрес -> функция-обработчик
  server.begin();
}

void loop() {
  server.handleClient();                     // ответить браузеру, если он что-то спросил
}
