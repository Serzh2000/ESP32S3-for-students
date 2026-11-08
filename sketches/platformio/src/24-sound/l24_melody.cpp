#include <Arduino.h>
const int BUZZER_PIN = 21;

struct Note {
  uint16_t freq;                          // частота в герцах; 0 -- пауза
  uint16_t ms;                            // длительность
};

const Note MELODY[] = {
  {262, 300}, {294, 300}, {330, 300}, {349, 300}, {392, 600}, {0, 200},
  {392, 300}, {349, 300}, {330, 300}, {294, 300}, {262, 600}, {0, 800}};
const int COUNT = sizeof(MELODY) / sizeof(MELODY[0]);
const uint16_t GAP_MS = 30;               // короткая тишина между нотами

int noteIndex = 0;
uint32_t noteStart = 0;
bool sounding = false;

void playMelody() {                       // вызывается из loop() и сразу возвращается
  const Note& n = MELODY[noteIndex];
  uint32_t t = millis() - noteStart;
  if (!sounding) {                        // начало ноты
    if (n.freq) tone(BUZZER_PIN, n.freq); else noTone(BUZZER_PIN);
    sounding = true;
    noteStart = millis();
  } else if (t >= n.ms) {                 // нота закончилась -- переходим к следующей
    noTone(BUZZER_PIN);
    if (t >= (uint32_t)n.ms + GAP_MS) {
      noteIndex = (noteIndex + 1) % COUNT;
      sounding = false;
    }
  }
}

void setup() {}

void loop() {
  playMelody();
  // здесь программа свободна: можно читать кнопки и датчики
}
