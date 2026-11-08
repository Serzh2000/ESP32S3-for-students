#include <Arduino.h>
// ESPUI 2.2.4: resources served from firmware; no filesystem upload.
#include <WiFi.h>
#include <ESPUI.h>
uint16_t uptimeId;
bool ready = false;
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  // Classroom demonstration password. Change for your own board.
  if (!WiFi.softAP("S3-ESPUI", "lesson-39-demo")) {
    Serial.println("AP failed"); return;
  }
  uptimeId = ESPUI.label("Uptime, s", ControlColor::Peterriver, "0");
  ESPUI.begin("My first ESPUI");
  ready = true;
  Serial.println(WiFi.softAPIP());
}
void loop() {
  static uint32_t last = 0;
  if (ready && millis() - last >= 1000) {
    last = millis();
    ESPUI.updateLabel(uptimeId, String(millis() / 1000));
  }
  delay(1);
}
