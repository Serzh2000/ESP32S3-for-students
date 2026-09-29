const int LED_PIN = 4;
const uint32_t FREQ = 5000;   // Гц
const uint8_t  RES  = 8;      // бит -> значения 0..255

void setup() {
  ledcAttach(LED_PIN, FREQ, RES);
}

void loop() {
  for (int d = 0; d <= 255; d++) { ledcWrite(LED_PIN, d); delay(4); }
  for (int d = 255; d >= 0; d--) { ledcWrite(LED_PIN, d); delay(4); }
}
