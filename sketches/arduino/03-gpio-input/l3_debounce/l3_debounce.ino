const int BTN_PIN = 5;
const uint32_t DEBOUNCE_MS = 50;

bool buttonPressed() {
  static bool stable = digitalRead(BTN_PIN);  // подтверждённое состояние
  static bool last = stable;                  // последнее прочитанное
  static uint32_t changed = 0;                // когда уровень менялся
  bool r = digitalRead(BTN_PIN);
  if (r != last) {                            // уровень дёрнулся --
    last = r;                                 // перезапускаем отсчёт
    changed = millis();
  }
  if (millis() - changed > DEBOUNCE_MS && r != stable) {
    stable = r;                               // уровень устоялся
    return stable == LOW;                     // событие -- только нажатие
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  if (buttonPressed()) Serial.println("Нажатие!");
}
