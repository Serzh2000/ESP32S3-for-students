// Плавная «волна» на четырёх светодиодах
const int LEDS[] = {4, 5, 6, 7};
const int COUNT = 4;

uint32_t gammaDuty(int percent) {
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void setup() {
  for (int i = 0; i < COUNT; i++) {
    ledcAttach(LEDS[i], 5000, 8);            // каждому светодиоду -- свой ШИМ
  }
}

void loop() {
  float t = millis() / 1000.0;               // время в секундах
  for (int i = 0; i < COUNT; i++) {
    // синусоида, сдвинутая для каждого светодиода на четверть периода
    float s = sin(2 * PI * (t - i * 0.25));  // от -1 до 1
    int percent = (s + 1) * 50;              // от 0 до 100
    ledcWrite(LEDS[i], gammaDuty(percent));
  }
  delay(10);
}
