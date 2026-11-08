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

 private:
  int pin_;
  bool stable_ = HIGH;
  bool last_ = HIGH;
  uint32_t changed_ = 0;
};

Button button(5);                         // кнопка на выводе 5

void setup() {
  Serial.begin(115200);
  button.begin();
}

void loop() {
  if (button.pressed()) Serial.println("Нажатие!");
}
