#include <Arduino.h>
// Панель, шаг 2: страница на HTML и JavaScript (без оформления) говорит с платой через /api.
#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 4;
WebServer server(80);
bool ledOn = false;
int level = 50;

const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Светодиод</title>
</head>
<body>
  <h1>Мой светодиод</h1>
  <p>Состояние: <b id="state">?</b></p>
  <button onclick="send('toggle')">Вкл / выкл</button>
  <input id="lvl" type="range" min="0" max="100" onchange="send('level=' + this.value)">
  <p>Яркость: <b id="txt">?</b> %</p>
<script>
function show(s) {                     // показать состояние на странице
  document.getElementById('state').textContent = s.on ? 'включён' : 'выключен';
  document.getElementById('lvl').value = s.level;
  document.getElementById('txt').textContent = s.level;
}
function send(cmd) {                   // POST-запрос без перезагрузки страницы
  fetch('/api', { method: 'POST', body: new URLSearchParams(cmd) })
    .then(r => r.json()).then(show);
}
fetch('/api').then(r => r.json()).then(show);   // GET: узнать состояние при открытии
</script>
</body>
</html>
)rawliteral";

void applyLed() { ledcWrite(LED_PIN, ledOn ? level * 255 / 100 : 0); }

void handleApi() {                           // тот же обработчик, что на шаге 1
  if (server.method() == HTTP_POST) {
    if (server.hasArg("toggle")) ledOn = !ledOn;
    if (server.hasArg("level")) { level = constrain(server.arg("level").toInt(), 0, 100); ledOn = true; }
    applyLed();
  }
  String json = String("{\"on\":") + (ledOn ? "true" : "false") + ",\"level\":" + level + "}";
  server.send(200, "application/json", json);
}

void setup() {
  ledcAttach(LED_PIN, 5000, 8);
  WiFi.softAP("S3-Web", "lesson-44-demo");
  server.on("/", []() { server.send(200, "text/html; charset=utf-8", PAGE); });
  server.on("/api", handleApi);
  server.begin();
}

void loop() {
  server.handleClient();
}
