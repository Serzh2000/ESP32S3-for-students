#include <Arduino.h>
// Шаг 2. Нажали кнопку GPIO5 (на GND) -- открылся портал настройки S3-Setup.
#include <WiFi.h>
#include <WiFiManager.h>
WiFiManager wm; // must outlive setup() in nonblocking mode

void openPortal() {
  wm.startConfigPortal("S3-Setup", "setup-40-demo");
  Serial.println("Setup window requested; see AP address below");
  Serial.println(WiFi.softAPIP());
}
void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT); digitalWrite(4, LOW);
  pinMode(5, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  wm.setDebugOutput(false); // do not print network secrets
  wm.setConfigPortalBlocking(false);
  wm.setConnectTimeout(10);
  WiFi.begin();
  Serial.println("Press GPIO5 button to configure Wi-Fi");
}
void loop() {
  wm.process();                              // обслуживает портал, если он открыт
  uint32_t now = millis();
  if (digitalRead(5) == LOW && !wm.getConfigPortalActive()) openPortal();
  static uint32_t last = 0;
  static bool led = false;
  if (now - last >= 500) {
    last = now; led = !led; digitalWrite(4, led);
  }
  static bool reported = false;
  if (WiFi.status() == WL_CONNECTED && !reported) {
    Serial.print("Connected: "); Serial.println(WiFi.localIP());
    reported = true;
  }
  if (WiFi.status() != WL_CONNECTED) reported = false;
  delay(1);
}
