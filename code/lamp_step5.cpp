// Умная лампа, шаг 5: ручка яркости
const int POT_PIN = 1;                       // ADC1 -- работает вместе с Wi-Fi

int readPotLevel() {                         // 0..100 %
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) sum += analogReadMilliVolts(POT_PIN);
  return constrain(map(sum / 8, 0, 3100, 0, 100), 0, 100);
}

int lastPot = -100;                          // «далеко» -> первое чтение применится

void loop() {
  handleSerial();
  if (buttonPressed()) lampToggle();
  int pot = readPotLevel();
  if (abs(pot - lastPot) >= 3) {             // реагируем, только если ручку повернули
    lastPot = pot;
    lampSetLevel(pot);
  }
  heartbeat();
}
