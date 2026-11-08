// Светодиод (GPIO4) управляется ссылками: /toggle переключает, /level?v=... задаёт яркость.
#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 4;
WebServer server(80);
bool ledOn = false;
int level = 50;                              // яркость, %

void applyLed() { ledcWrite(LED_PIN, ledOn ? level * 255 / 100 : 0); }

String page() {                              // собираем HTML-страницу из текущего состояния
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width'></head><body>";
  html += String("<h1>Светодиод: ") + (ledOn ? "ВКЛ" : "ВЫКЛ") + "</h1>";
  html += "<p><a href='/toggle'><button>Вкл/выкл</button></a></p>"
          "<form action='/level'>Яркость: <input type='range' name='v' min='0' max='100' value='"
          + String(level) + "' onchange='this.form.submit()'></form></body></html>";
  return html;
}

void goHome() {                              // «перейди на главную страницу»
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  ledcAttach(LED_PIN, 5000, 8);
  WiFi.softAP("S3-Web", "lesson-44-demo");
  server.on("/",       []() { server.send(200, "text/html; charset=utf-8", page()); });
  server.on("/toggle", []() { ledOn = !ledOn; applyLed(); goHome(); });
  server.on("/level",  []() {
    level = constrain(server.arg("v").toInt(), 0, 100);
    applyLed();
    goHome();
  });
  server.begin();
}

void loop() {
  server.handleClient();
}
