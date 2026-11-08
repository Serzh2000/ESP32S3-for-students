#include "USB.h"
#include "USBHIDKeyboard.h"

const int BUTTON_PIN = 5;                 // кнопка между GPIO5 и GND
USBHIDKeyboard Keyboard;

bool wasPressed = false;
uint32_t lastChange = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Keyboard.begin();
  USB.begin();
}

void loop() {
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  if (pressed != wasPressed && millis() - lastChange > 30) {   // защита от дребезга
    lastChange = millis();
    wasPressed = pressed;
    if (pressed) {                        // печатаем один раз на нажатие
      Keyboard.print("Hello from ESP32-S3");  // только латиница и цифры
      Keyboard.write(KEY_RETURN);         // клавиша Enter
    }
  }
}
