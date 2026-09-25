// Умная лампа, шаг 8: управление через браузер
#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
WebServer server(80);

String page() {
  LampState s = getLamp();
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width'>"
                "<title>Лампа</title></head><body style='font-family:sans-serif'>";
  html += "<h1>Лампа: ";
  html += s.on ? "ВКЛ" : "ВЫКЛ";
  html += "</h1><p><a href='/toggle'><button>Вкл/выкл</button></a></p>"
          "<form action='/level'>Яркость: <input type='range' name='v' "
          "min='0' max='100' value='" + String(s.level) + "' "
          "onchange='this.form.submit()'></form></body></html>";
  return html;
}

void redirectHome() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void setupWeb() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);                    // не ждём: лампа работает и без сети
  server.on("/",       []() { server.send(200, "text/html; charset=utf-8", page()); });
  server.on("/toggle", []() { lampToggle(); redirectHome(); });
  server.on("/level",  []() { lampSetLevel(server.arg("v").toInt()); redirectHome(); });
  server.begin();
}

// setup(): в конце добавьте вызов setupWeb()
// drawDisplay(): в заголовке показываем адрес, когда сеть подключена:
//   display.print(WiFi.isConnected() ? WiFi.localIP().toString() : String("Smart lamp"));

void loop() {
  handleSerial();
  server.handleClient();                     // обработать входящие запросы
  heartbeat();
  delay(2);
}
