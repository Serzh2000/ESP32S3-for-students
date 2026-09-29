// Красивая панель управления светодиодом: HTML + CSS + немного JavaScript
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
const int LED_PIN = 4;

WebServer server(80);
bool ledOn = false;
int level = 50;                              // яркость, %

// Вся страница -- одна большая строка. R"rawliteral( ... )rawliteral"
// позволяет писать кавычки и переносы строк без лишних значков.
const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Светодиод</title>
<style>
  body  { font-family: sans-serif; background: #eef2f7;
          display: flex; justify-content: center; padding-top: 40px; }
  .card { background: white; border-radius: 20px; padding: 24px 32px; width: 260px;
          box-shadow: 0 6px 20px rgba(0,0,0,.12); text-align: center; }
  h1    { color: #7b241c; font-size: 24px; }
  .led { width: 90px; height: 90px; border-radius: 50%; margin: 10px auto 20px;
          background: #ccc; transition: .3s; }
  .led.on { background: #ff5a4f; box-shadow: 0 0 30px #ff5a4f; }
  button { background: #1f618d; color: white; border: none; border-radius: 12px;
           font-size: 18px; padding: 10px 28px; }
  input  { width: 100%; margin-top: 20px; }
</style>
</head>
<body>
<div class="card">
  <h1>Мой светодиод</h1>
  <div id="led" class="led"></div>
  <button onclick="send('toggle')">Вкл / выкл</button>
  <input id="lvl" type="range" min="0" max="100" onchange="send('level=' + this.value)">
  <p>Яркость: <b id="txt">?</b> %</p>
</div>
<script>
function show(s) {                     // нарисовать состояние светодиода
  document.getElementById('led').className = s.on ? 'led on' : 'led';
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

uint32_t gammaDuty(int percent) {
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void applyLed() { ledcWrite(LED_PIN, ledOn ? gammaDuty(level) : 0); }

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
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);
  applyLed();
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.printf("\nОткрой http://%s или http://esp32-led.local\n",
                WiFi.localIP().toString().c_str());
  MDNS.begin("esp32-led");

  server.on("/", []() { server.send(200, "text/html; charset=utf-8", PAGE); });
  server.on("/api", handleApi);
  server.begin();
}

void loop() {
  server.handleClient();
}
