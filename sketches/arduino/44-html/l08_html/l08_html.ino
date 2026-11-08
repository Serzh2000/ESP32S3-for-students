// Страница, написанная на HTML: заголовок, абзац, список, ссылка и кнопка. Пока без оформления.
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// R"rawliteral( ... )rawliteral" позволяет писать кавычки и переносы строк без лишних значков.
const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Моя плата</title>
</head>
<body>
  <h1>Моя плата</h1>
  <p>Эта страница лежит в памяти <b>ESP32-S3</b>.</p>
  <ul>
    <li>процессор: 2 ядра</li>
    <li>связь: Wi-Fi и Bluetooth</li>
  </ul>
  <p><a href="https://www.espressif.com">Сайт производителя</a></p>
  <button>Кнопка (пока ничего не делает)</button>
</body>
</html>
)rawliteral";

void setup() {
  WiFi.softAP("S3-Web", "lesson-44-demo");
  server.on("/", []() { server.send(200, "text/html; charset=utf-8", PAGE); });
  server.begin();
}

void loop() {
  server.handleClient();
}
