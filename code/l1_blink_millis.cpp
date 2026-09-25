const int LED_PIN = 4;
const uint32_t INTERVAL = 500;  // мс
uint32_t lastToggle = 0;
bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uint32_t now = millis();
  if (now - lastToggle >= INTERVAL) {  // корректно и при переполнении millis()
    lastToggle = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
  // здесь можно делать что-то ещё, не дожидаясь светодиода
}
