#include "USB.h"
#include "USBHIDMouse.h"

const int BUTTON_PIN = 5;                 // пока кнопка нажата, курсор «рисует» квадрат
USBHIDMouse Mouse;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Mouse.begin();
  USB.begin();
}

void loop() {
  if (digitalRead(BUTTON_PIN) != LOW) {   // кнопка отпущена -- ничего не делаем
    delay(20);
    return;
  }
  const int8_t dx[4] = { 5, 0, -5, 0 };   // вправо, вниз, влево, вверх
  const int8_t dy[4] = { 0, 5, 0, -5 };
  for (int side = 0; side < 4; side++) {
    for (int i = 0; i < 20; i++) {
      if (digitalRead(BUTTON_PIN) != LOW) return;   // отпустили -- сразу остановиться
      Mouse.move(dx[side], dy[side]);
      delay(10);
    }
  }
}
