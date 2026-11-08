#include <Arduino.h>
const int BUZZER_PIN = 21;   // пассивная пьезопищалка между GPIO21 и GND
const int BTN_PIN = 5;

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  static bool wasPressed = false;
  bool pressed = (digitalRead(BTN_PIN) == LOW);
  if (pressed && !wasPressed) {
    tone(BUZZER_PIN, 1000, 150);         // 1000 Гц на 150 мс: короткий «бип»
  }
  wasPressed = pressed;
  delay(20);
}
