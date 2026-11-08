#include "Debounce.h"

bool Debounce::update(bool raw, uint32_t nowMs) {
  if (raw != last_) {                     // уровень изменился: начинаем отсчёт заново
    last_ = raw;
    changed_ = nowMs;
  }
  if (raw != stable_ && nowMs - changed_ >= stableMs_) {
    stable_ = raw;                        // уровень продержался достаточно долго
    return stable_ == false;              // событие только для нажатия, не для отпускания
  }
  return false;
}
