#include <Arduino.h>
// Шаг 1 (урок 25): вал крутится без остановки. ULN2003: IN1..IN4 -> GPIO15..GPIO18.
const int PINS[4] = {15, 16, 17, 18};     // входы IN1..IN4 платы ULN2003

// Полушаговый режим: 8 фаз. Бит 0 -> IN1, бит 1 -> IN2, бит 2 -> IN3, бит 3 -> IN4
const uint8_t PHASES[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};

const uint32_t STEP_US = 1200;            // пауза между шагами: меньше -- быстрее, но слабее

int phase = 0;
uint32_t lastStep = 0;

void applyPhase(int p) {                  // включить обмотки по таблице фаз
  for (int i = 0; i < 4; i++) digitalWrite(PINS[i], (PHASES[p] >> i) & 1);
}

void setup() {
  for (int i = 0; i < 4; i++) pinMode(PINS[i], OUTPUT);
}

void loop() {
  if (micros() - lastStep >= STEP_US) {   // пора делать следующий шаг
    lastStep = micros();
    phase = (phase + 1) % 8;
    applyPhase(phase);
  }
}
