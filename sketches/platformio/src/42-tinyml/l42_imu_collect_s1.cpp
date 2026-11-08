#include <Arduino.h>
// Шаг 1. Чтение акселерометра MPU6050 (SDA GPIO8, SCL GPIO9) 50 раз в секунду.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const uint32_t PERIOD_MS = 20;              // 50 отсчётов в секунду
uint32_t nextMs = 0;

bool readAccel(float* g) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);                         // регистр ACCEL_XOUT_H
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, (size_t)6) != 6) return false;
  for (int i = 0; i < 3; ++i) {
    uint8_t hi = Wire.read();               // порядок чтения важен: сначала старший байт
    uint8_t lo = Wire.read();
    int16_t raw = (int16_t)((hi << 8) | lo);
    g[i] = raw / 16384.0f;                  // диапазон ±2 g: 16384 отсчёта на 1 g
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);                            // разбудить датчик
  Wire.endTransmission();
}

void loop() {
  if (millis() < nextMs) return;
  nextMs += PERIOD_MS;
  if (nextMs < millis()) nextMs = millis() + PERIOD_MS;   // отстали -- не догоняем

  float g[3];
  if (!readAccel(g)) { Serial.println("sensor error"); return; }
  Serial.printf("%.3f %.3f %.3f\n", g[0], g[1], g[2]);
}
