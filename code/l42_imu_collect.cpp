// Урок 42. Сбор данных: окно 1 с с MPU6050 (SDA GPIO8, SCL GPIO9) -> строка CSV.
// Команды из Serial Monitor: цифра 0..9 -- номер класса, n -- новый сеанс записи.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const int WINDOW = 50;                      // 50 отсчётов по 20 мс = 1 секунда
const uint32_t PERIOD_MS = 20;

float samples[WINDOW][3];                   // ускорения по трём осям, в g
int count = 0;
int label = 0;
int session = 0;
uint32_t nextMs = 0;

bool readAccel(float* g) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);                         // регистр ACCEL_XOUT_H
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, (size_t)6) != 6) return false;
  for (int i = 0; i < 3; ++i) {
    uint8_t hi = Wire.read();                 // порядок чтения важен: сначала старший байт
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
  Serial.println("# session,label,mx,my,mz,sx,sy,sz");
  Serial.println("# digit = class, n = new session");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c >= '0' && c <= '9') { label = c - '0'; Serial.printf("# class %d\n", label); }
    if (c == 'n') { ++session; Serial.printf("# session %d\n", session); }
  }
  if (millis() < nextMs) return;
  nextMs += PERIOD_MS;
  if (nextMs < millis()) nextMs = millis() + PERIOD_MS;   // отстали -- не догоняем

  if (!readAccel(samples[count])) { Serial.println("# sensor error"); return; }
  if (++count < WINDOW) return;
  count = 0;

  float mean[3] = {0, 0, 0}, sd[3] = {0, 0, 0};
  for (int a = 0; a < 3; ++a) {
    for (int i = 0; i < WINDOW; ++i) mean[a] += samples[i][a];
    mean[a] /= WINDOW;
    for (int i = 0; i < WINDOW; ++i) sd[a] += sq(samples[i][a] - mean[a]);
    sd[a] = sqrtf(sd[a] / WINDOW);
  }
  Serial.printf("%d,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", session, label,
                mean[0], mean[1], mean[2], sd[0], sd[1], sd[2]);
}
