#include <Arduino.h>
const int ROWS[4] = {15, 16, 17, 18};       // строки клавиатуры
const int COLS[4] = {6, 7, 21, 47};         // столбцы клавиатуры
const char KEYS[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};

char scanKeypad() {                         // 0, если ничего не нажато
  char found = 0;
  for (int r = 0; r < 4; r++) {
    pinMode(ROWS[r], OUTPUT);               // активна только одна строка
    digitalWrite(ROWS[r], LOW);
    delayMicroseconds(50);                  // дать уровню установиться
    for (int c = 0; c < 4; c++) {
      if (digitalRead(COLS[c]) == LOW) found = KEYS[r][c];
    }
    pinMode(ROWS[r], INPUT);                // остальные строки «отключены»
  }
  return found;
}

void setup() {
  Serial.begin(115200);
  for (int c = 0; c < 4; c++) pinMode(COLS[c], INPUT_PULLUP);
  for (int r = 0; r < 4; r++) pinMode(ROWS[r], INPUT);
}

void loop() {
  static char last = 0;
  char key = scanKeypad();
  if (key && key != last) Serial.printf("Клавиша: %c\n", key);  // только новое нажатие
  last = key;
  delay(20);                                // опрос 50 раз в секунду гасит дребезг
}
