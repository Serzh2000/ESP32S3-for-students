// «Дыхание» с поправкой на глаз (гамма-коррекция)
const int LED_PIN = 4;

uint32_t gammaDuty(int percent) {            // 0..100 % -> 0..255
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void setup() {
  ledcAttach(LED_PIN, 5000, 8);
}

void loop() {
  for (int p = 0; p <= 100; p++)  { ledcWrite(LED_PIN, gammaDuty(p)); delay(10); }
  for (int p = 100; p >= 0; p--)  { ledcWrite(LED_PIN, gammaDuty(p)); delay(10); }
}
