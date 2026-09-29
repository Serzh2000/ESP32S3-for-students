// Управляем светодиодом через GET-запросы: /toggle и /level?v=...
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* SSID = "MyNetwork";              // имя твоей Wi-Fi сети
const char* PASS = "MyPassword";             // и пароль
const int LED_PIN = 4;

WebServer server(80);
bool ledOn = false;
int level = 50;                              // яркость, %

uint32_t gammaDuty(int percent) {
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void applyLed() { ledcWrite(LED_PIN, ledOn ? gammaDuty(level) : 0); }

String page() {                              // собираем HTML-страницу
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width'>"
                "<title>Светодиод</title></head><body style='font-family:sans-serif'>";
  html += "<h1>Светодиод: ";
  html += ledOn ? "ВКЛ" : "ВЫКЛ";
  html += "</h1><p><a href='/toggle'><button>Вкл/выкл</button></a></p>"
          "<form action='/level'>Яркость: <input type='range' name='v' "
          "min='0' max='100' value='" + String(level) + "' "
          "onchange='this.form.submit()'></form></body></html>";
  return html;
}

void goHome() {                              // «перейди на главную страницу»
  server.sendHeader("Location", "/");
  server.send(303);
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
  server.handleClient();                     // ответить браузеру, если он что-то спросил
}
