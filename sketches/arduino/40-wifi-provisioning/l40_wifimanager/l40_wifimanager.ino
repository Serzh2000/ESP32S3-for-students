// Button GPIO5 -> GND. Hold for 2 s to open a 120 s setup window.
// LED GPIO4 through a resistor; standalone example, no ESPUI server.
#include <WiFi.h>
#include <WiFiManager.h>
WiFiManager wm; // must outlive setup() in nonblocking mode
uint32_t portalStarted = 0;
bool buttonHeld = false, handled = false;
uint32_t pressedAt = 0;

void openPortal() {
  wm.startConfigPortal("S3-Setup", "setup-40-demo");
  portalStarted = millis();
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
  WiFi.begin(); // try credentials already stored on the board
  Serial.println("Hold GPIO5 button for 2 s to configure Wi-Fi");
}
void loop() {
  wm.process();
  uint32_t now = millis();
  if (wm.getConfigPortalActive() && now - portalStarted >= 120000) {
    wm.stopConfigPortal();
    Serial.println("Setup window closed");
  }
  if (digitalRead(5) == LOW) {
    if (!buttonHeld) {
      buttonHeld = true; handled = false; pressedAt = now;
    }
    if (!handled && now - pressedAt >= 2000) {
      handled = true;
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
