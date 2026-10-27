#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
const int LED_PIN = 4;

WebServer server(80);
bool ledOn = false;
int level = 50;                                  // яркость, %

void applyLed() {
  ledcWrite(LED_PIN, ledOn ? level * 255 / 100 : 0);
}

void handleRoot() {                              // страница строится из текущего состояния
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width'></head><body>"
                "<h2>ESP32-S3</h2><p>LED: ";
  html += ledOn ? "ВКЛ" : "ВЫКЛ";
  html += "</p><p><a href='/toggle'>Вкл/выкл</a></p>"
          "<form action='/level'><input type='range' name='v' min='0' max='100' value='";
  html += level;
  html += "' onchange='this.form.submit()'></form></body></html>";
  server.send(200, "text/html", html);
}

void redirectHome() {                            // 303: браузер снова запросит "/"
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleToggle() {
  ledOn = !ledOn;
  applyLed();
  redirectHome();
}

void handleLevel() {
  if (server.hasArg("v")) {
    level = constrain(server.arg("v").toInt(), 0, 100);
    applyLed();
  }
  redirectHome();
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  Serial.println(WiFi.localIP());                // этот адрес откройте в браузере

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/level", handleLevel);
  server.begin();
}

void loop() {
  server.handleClient();                         // обработать входящие запросы
  delay(2);
}
