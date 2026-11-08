#include <Arduino.h>
const int DATA_PIN  = 15;   // DS   (вывод 14 микросхемы 74HC595)
const int CLOCK_PIN = 16;   // SHCP (вывод 11)
const int LATCH_PIN = 7;    // STCP (вывод 12)

// Сегменты: бит 0 = a, бит 1 = b, ..., бит 6 = g, бит 7 = точка
const uint8_t DIGITS[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

void show(uint8_t pattern) {
  digitalWrite(LATCH_PIN, LOW);                      // защёлка закрыта: выходы не меняются
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);  // 8 бит по одному, старший первым
  digitalWrite(LATCH_PIN, HIGH);                     // все 8 выходов обновились разом
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
}

void loop() {
  for (int d = 0; d <= 9; d++) {
    show(DIGITS[d]);
    delay(600);
    show(DIGITS[d] | 0b10000000);                    // та же цифра с точкой
    delay(200);
  }
}
