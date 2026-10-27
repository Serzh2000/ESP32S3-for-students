#include "USB.h"
#include "USBHIDConsumerControl.h"

const int POT_PIN = 1;                    // потенциометр на GPIO1
const int BUTTON_PIN = 5;                 // кнопка: включить/выключить звук
const int STEP = 128;                     // 4096 / 128 = 32 положения ручки

USBHIDConsumerControl Consumer;
int lastPos = 0;
bool wasPressed = false;

void tap(uint16_t key) {                  // нажать и сразу отпустить мультимедийную клавишу
  Consumer.press(key);
  Consumer.release();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  analogReadResolution(12);
  Consumer.begin();
  USB.begin();
  lastPos = analogRead(POT_PIN) / STEP;   // запоминаем стартовое положение
}

void loop() {
  int pos = analogRead(POT_PIN) / STEP;
  while (pos > lastPos) { tap(CONSUMER_CONTROL_VOLUME_INCREMENT); lastPos++; }  // громче
  while (pos < lastPos) { tap(CONSUMER_CONTROL_VOLUME_DECREMENT); lastPos--; }  // тише

  bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  if (pressed && !wasPressed) tap(CONSUMER_CONTROL_MUTE);   // нажатие: звук вкл/выкл
  wasPressed = pressed;
  delay(20);
}
