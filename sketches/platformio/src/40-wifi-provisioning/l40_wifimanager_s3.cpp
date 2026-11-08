#include <Arduino.h>
// Шаг 3. Портал открывается только после удержания кнопки 2 секунды.
#include <WiFi.h>
#include <WiFiManager.h>
WiFiManager wm; // must outlive setup() in nonblocking mode
bool buttonHeld = false, handled = false;
uint32_t pressedAt = 0;

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
  Serial.println("Hold GPIO5 button for 2 s to configure Wi-Fi");
}
void loop() {
  wm.process();
  uint32_t now = millis();
  if (digitalRead(5) == LOW) {
    if (!buttonHeld) {                       // кнопку только что нажали
      buttonHeld = true; handled = false; pressedAt = now;
    }
    if (!handled && now - pressedAt >= 2000) {
      handled = true;                        // одно удержание -- одна попытка
      if (!wm.getConfigPortalActive()) openPortal();
    }
  } else {
    buttonHeld = false; handled = false;
  }
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
