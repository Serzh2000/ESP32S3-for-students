class Led {
 public:
  explicit Led(int pin) : pin_(pin) {}
  void begin() { pinMode(pin_, OUTPUT); }
  void set(bool on) { on_ = on; digitalWrite(pin_, on_); }
  void toggle() { set(!on_); }

 private:                                 // снаружи эти переменные недоступны
  int pin_;
  bool on_ = false;
};

Led led(4);                               // объект: светодиод на выводе 4

uint32_t lastToggle = 0;

void setup() {
  led.begin();
}

void loop() {
  if (millis() - lastToggle >= 500) {     // мигаем без delay()
    lastToggle = millis();
    led.toggle();
  }
}
