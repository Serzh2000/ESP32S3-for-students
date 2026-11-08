const int BTN_PIN = 5;

volatile uint32_t pressCount = 0;   // volatile: меняется в обработчике
volatile uint32_t lastIsrTime = 0;

void IRAM_ATTR onButton() {         // IRAM_ATTR: код в быстрой памяти
  uint32_t now = millis();
  if (now - lastIsrTime > 50) {     // грубое подавление дребезга
    pressCount++;
    lastIsrTime = now;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButton, FALLING);
}

void loop() {
  static uint32_t shown = 0;
  if (pressCount != shown) {        // печатаем здесь, а не в обработчике
    shown = pressCount;
    Serial.printf("Нажатий: %lu\n", shown);
  }
}
