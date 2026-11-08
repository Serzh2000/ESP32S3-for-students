#include <Arduino.h>
// Шаг 2 (урок 25): считаем шаги и останавливаемся ровно в цели -- один оборот.
const int PINS[4] = {15, 16, 17, 18};     // входы IN1..IN4 платы ULN2003

// Полушаговый режим: 8 фаз. Бит 0 -> IN1, бит 1 -> IN2, бит 2 -> IN3, бит 3 -> IN4
const uint8_t PHASES[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};

const long STEPS_PER_REV = 4096;          // полушагов на оборот вала 28BYJ-48 (примерно)
const uint32_t STEP_US = 1200;            // пауза между шагами: меньше -- быстрее, но слабее

long current = 0;                         // где находимся, в шагах
long target = 0;                          // куда нужно прийти
uint32_t lastStep = 0;

void applyPhase(int phase) {
  for (int i = 0; i < 4; i++) digitalWrite(PINS[i], (PHASES[phase] >> i) & 1);
}

void release() {                          // снять ток с обмоток: мотор не греется
  for (int i = 0; i < 4; i++) digitalWrite(PINS[i], LOW);
}

void stepperRun() {                       // один шаг за вызов, без delay()
  if (current == target) return;
  if (micros() - lastStep < STEP_US) return;
  lastStep = micros();
  current += (target > current) ? 1 : -1;
  applyPhase(((current % 8) + 8) % 8);
  if (current == target) release();
}

void setup() {
  for (int i = 0; i < 4; i++) pinMode(PINS[i], OUTPUT);
  target = STEPS_PER_REV;                 // один оборот вперёд
}

void loop() {
  stepperRun();
}
