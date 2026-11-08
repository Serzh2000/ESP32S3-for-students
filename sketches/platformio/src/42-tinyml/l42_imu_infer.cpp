#include <Arduino.h>
// Урок 42. Распознавание жестов на устройстве: окно 1 с -> 6 признаков -> сеть 6-8-3.
// Блок модели создаёт examples/tinyml/train_gesture.py; здесь стоит модель, обученная на демо-данных.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const int WINDOW = 50;
const uint32_t PERIOD_MS = 20;
const float MIN_CONFIDENCE = 0.80f;         // ниже -- "не уверен", класс не объявляем
const int CONFIRM = 3;                      // сколько окон подряд должны согласиться

// ---- НАЧАЛО СГЕНЕРИРОВАННОЙ МОДЕЛИ ----
// классы: 0=покой, 1=тряска, 2=наклон
const int NC = 3;
// слой 1: 6 -> 8
const int8_t W1[48] = {
  41, 10, -9, -72, -14, -9, 15, -12,
  -36, 56, 32, 65, -77, 23, -60, -27,
  48, -54, -58, -61, 48, -39, 89, 77,
  -111, -53, -28, -42, -35, 17, 127, -47,
  -29, 89, 2, -4, -60, -50, 59, -4,
  -60, -9, -24, -61, -46, 13, 55, -27
};
const float S1 = 0.01629021f;
const float BIAS1[8] = {
  0.18936f, 0.16731f, 0.57299f, 0.46070f, 0.83547f, -0.11008f, 0.74104f, 0.07001f
};
// слой 2: 8 -> 3
const int8_t W2[24] = {
  92, -3, 21, -100, 18, 39, -16, -100,
  99, -41, -102, 27, 80, -59, -127, -64,
  -16, 35, -57, 102, -116, 63, 14, -90
};
const float S2 = 0.01073441f;
const float BIAS2[3] = {
  0.36822f, -0.36463f, 0.23739f
};
// всего весов: 72 (int8: 72 байт; во float было бы 288)

const float FMEAN[6] = { 0.00933f, 0.21500f, 0.87735f, 0.16119f, 0.15984f, 0.12908f };
const float FSTD[6] = { 0.04454f, 0.34230f, 0.16016f, 0.20487f, 0.20293f, 0.15954f };
// ---- КОНЕЦ СГЕНЕРИРОВАННОЙ МОДЕЛИ ----

const char* NAMES[] = {"покой", "тряска", "наклон"};

float samples[WINDOW][3];
int count = 0;
uint32_t nextMs = 0;

bool readAccel(float* g) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, (size_t)6) != 6) return false;
  for (int i = 0; i < 3; ++i) {
    uint8_t hi = Wire.read();                 // порядок чтения важен: сначала старший байт
    uint8_t lo = Wire.read();
    int16_t raw = (int16_t)((hi << 8) | lo);
    g[i] = raw / 16384.0f;
  }
  return true;
}

void dense(const float* in, int nIn, const int8_t* w, float scale,
           const float* b, float* out, int nOut, bool relu) {
  for (int j = 0; j < nOut; ++j) {
    float sum = 0;
    for (int i = 0; i < nIn; ++i) sum += in[i] * w[i * nOut + j];
    sum = sum * scale + b[j];
    out[j] = (relu && sum < 0) ? 0 : sum;
  }
}

// Возвращает номер класса и уверенность (0..1); softmax превращает ответы сети в доли.
int classify(const float* feat, float* confidence) {
  float in[6], h[8], z[NC];
  for (int i = 0; i < 6; ++i) in[i] = (feat[i] - FMEAN[i]) / FSTD[i];   // как при обучении
  dense(in, 6, W1, S1, BIAS1, h, 8, true);
  dense(h, 8, W2, S2, BIAS2, z, NC, false);
  int best = 0;
  for (int k = 1; k < NC; ++k) if (z[k] > z[best]) best = k;
  float sum = 0;
  for (int k = 0; k < NC; ++k) sum += expf(z[k] - z[best]);   // вычитаем максимум -- так надёжнее
  *confidence = 1.0f / sum;
  return best;
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
  if (++count < WINDOW) return;
  count = 0;

  float feat[6] = {0, 0, 0, 0, 0, 0};       // 0..2 -- средние, 3..5 -- разброс
  for (int a = 0; a < 3; ++a) {
    for (int i = 0; i < WINDOW; ++i) feat[a] += samples[i][a];
    feat[a] /= WINDOW;
    for (int i = 0; i < WINDOW; ++i) feat[3 + a] += sq(samples[i][a] - feat[a]);
    feat[3 + a] = sqrtf(feat[3 + a] / WINDOW);
  }

  uint32_t t0 = micros();
  float conf;
  int cls = classify(feat, &conf);
  uint32_t us = micros() - t0;

  static int lastCls = -1, streak = 0;
  int shown = (conf >= MIN_CONFIDENCE) ? cls : -1;      // -1 -- «неизвестно»
  streak = (shown == lastCls) ? streak + 1 : 1;
  lastCls = shown;

  Serial.printf("class:%s conf:%.2f time:%u us%s\n",
                shown < 0 ? "?" : NAMES[shown], conf, (unsigned)us,
                (shown >= 0 && streak >= CONFIRM) ? "  <-- ПОДТВЕРЖДЕНО" : "");
}
