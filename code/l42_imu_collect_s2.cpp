// Шаг 2. Окно из 50 отсчётов (1 секунда) и признак «среднее» по каждой оси.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const int WINDOW = 50;                      // 50 отсчётов по 20 мс = 1 секунда
const uint32_t PERIOD_MS = 20;

float samples[WINDOW][3];                   // ускорения по трём осям, в g
int count = 0;
uint32_t nextMs = 0;

bool readAccel(float* g) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);                         // регистр ACCEL_XOUT_H
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, (size_t)6) != 6) return false;
  for (int i = 0; i < 3; ++i) {
    uint8_t hi = Wire.read();
    uint8_t lo = Wire.read();
    int16_t raw = (int16_t)((hi << 8) | lo);
    g[i] = raw / 16384.0f;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
}

void loop() {
  if (millis() < nextMs) return;
  nextMs += PERIOD_MS;
  if (nextMs < millis()) nextMs = millis() + PERIOD_MS;

  if (!readAccel(samples[count])) { Serial.println("sensor error"); return; }
  if (++count < WINDOW) return;             // окно ещё не заполнено
  count = 0;

  float mean[3] = {0, 0, 0};
  for (int a = 0; a < 3; ++a) {
    for (int i = 0; i < WINDOW; ++i) mean[a] += samples[i][a];
    mean[a] /= WINDOW;
  }
  Serial.printf("%.4f,%.4f,%.4f\n", mean[0], mean[1], mean[2]);
}
