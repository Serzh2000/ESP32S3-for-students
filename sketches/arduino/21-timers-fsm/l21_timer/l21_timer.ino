const int LED_PIN = 4;

hw_timer_t* timer = nullptr;
volatile uint32_t ticks = 0;                 // меняется в обработчике

void ARDUINO_ISR_ATTR onTimer() {            // вызывается ровно раз в 0,5 с
  ticks++;
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  timer = timerBegin(1000000);               // счётчик: 1 000 000 тиков в секунду
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 500000, true, 0);        // сигнал каждые 500 000 тиков, без конца
}

void loop() {
  static uint32_t shown = 0;
  uint32_t now = ticks;
  if (now != shown) {                        // печатаем здесь, а не в обработчике
    shown = now;
    Serial.printf("Срабатываний: %lu\n", now);
  }
}
