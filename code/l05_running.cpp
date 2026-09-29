// Бегущий огонь: четыре светодиода загораются по очереди
const int LEDS[] = {4, 5, 6, 7};             // выводы светодиодов
const int COUNT = 4;                         // сколько их
const uint32_t STEP_MS = 150;                // время горения одного, мс

int current = 0;                             // какой светодиод горит сейчас
uint32_t lastStep = 0;                       // когда было последнее переключение

void setup() {
  for (int i = 0; i < COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  if (millis() - lastStep >= STEP_MS) {      // пора переключаться?
    lastStep = millis();
    digitalWrite(LEDS[current], LOW);        // гасим текущий
    current = (current + 1) % COUNT;         // следующий: 0,1,2,3,0,1...
    digitalWrite(LEDS[current], HIGH);       // зажигаем его
  }
}
