#pragma once
#include <stdint.h>

// Подавление дребезга. Класс не знает про Arduino: время ему передают снаружи,
// поэтому его можно проверять модульными тестами на компьютере.
class Debounce {
 public:
  explicit Debounce(uint32_t stableMs) : stableMs_(stableMs) {}

  // raw -- текущий уровень на входе (true = HIGH), nowMs -- текущее время в мс.
  // Возвращает true ровно один раз: когда нажатие (уровень LOW) признано устойчивым.
  bool update(bool raw, uint32_t nowMs);

 private:
  uint32_t stableMs_;
  bool stable_ = true;      // подтянутый вход без нажатия читается как HIGH
  bool last_ = true;
  uint32_t changed_ = 0;
};
