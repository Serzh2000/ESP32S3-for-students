#include <Arduino.h>
class Button {                            // кнопка с подавлением дребезга (урок 3)
 public:
  explicit Button(int pin) : pin_(pin) {}

  void begin() {
    pinMode(pin_, INPUT_PULLUP);
    stable_ = last_ = digitalRead(pin_);
  }

  bool pressed() {                        // true ровно один раз на нажатие
    bool r = digitalRead(pin_);
    if (r != last_) { last_ = r; changed_ = millis(); }
    if (millis() - changed_ > 50 && r != stable_) {
      stable_ = r;
      return stable_ == LOW;
    }
    return false;
  }

 private:                                 // снаружи эти переменные недоступны
  int pin_;
  bool stable_ = HIGH;
  bool last_ = HIGH;
  uint32_t changed_ = 0;
};

class Led {
 public:
  explicit Led(int pin) : pin_(pin) {}
  void begin() { pinMode(pin_, OUTPUT); }
  void set(bool on) { on_ = on; digitalWrite(pin_, on_); }
  void toggle() { set(!on_); }

 private:
  int pin_;
  bool on_ = false;
};

Button button(5);                         // два объекта -- и никаких глобальных флагов
Led led(4);

void setup() {
  button.begin();
  led.begin();
}

void loop() {
  if (button.pressed()) led.toggle();
}
