const int TOUCH_PIN = 14;                 // сенсорные входы ESP32-S3: GPIO1..GPIO14
const int LED_PIN = 4;

uint32_t threshold = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);

  uint32_t base = 0;                      // калибровка: не трогайте провод при запуске
  for (int i = 0; i < 16; i++) {
    base += touchRead(TOUCH_PIN);
    delay(10);
  }
  base /= 16;
  threshold = base + base / 10;           // порог на 10 % выше «спокойного» уровня
  Serial.printf("Базовый уровень: %lu, порог: %lu\n", base, threshold);
}

void loop() {
  uint32_t value = touchRead(TOUCH_PIN);  // на ESP32-S3 значение РАСТЁТ при касании
  digitalWrite(LED_PIN, value > threshold);
  Serial.printf("touch:%lu порог:%lu\n", value, threshold);
  delay(100);
}
