#include <Arduino.h>
#include <Debounce.h>
#include "config.h"

Debounce button(DEBOUNCE_MS);
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("PlatformIO: lamp ready");
}

void loop() {
  if (button.update(digitalRead(BUTTON_PIN), millis())) {
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn);
    Serial.printf("lamp %s\n", ledOn ? "ON" : "OFF");
  }
}
