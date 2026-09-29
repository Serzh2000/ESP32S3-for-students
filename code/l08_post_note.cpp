// Записка для платы: форма отправляет текст методом POST
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

WebServer server(80);
String note = "(записок пока нет)";          // последняя полученная записка

void handleRoot() {                          // GET /  -- показать страницу с формой
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body>"
                "<h1>Записка для ESP32</h1>"
                "<p>Последняя записка: <b>" + note + "</b></p>"
                "<form method='POST' action='/note'>"
                "<input name='text' placeholder='Напиши что-нибудь'>"
                "<button>Отправить</button></form></body></html>";
  server.send(200, "text/html; charset=utf-8", html);
}

void handleNote() {                          // POST /note -- принять записку
  note = server.arg("text");                 // данные из тела запроса
  Serial.println("Новая записка: " + note);
  server.sendHeader("Location", "/");        // вернуться на главную
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.printf("\nОткрой http://%s или http://esp32.local\n",
                WiFi.localIP().toString().c_str());
  MDNS.begin("esp32");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/note", HTTP_POST, handleNote); // этот адрес принимает только POST
  server.begin();
}

void loop() {
  server.handleClient();
}
