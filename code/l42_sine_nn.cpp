// Урок 42. Сеть 1-8-8-1 повторяет sin(x); яркость светодиода следует за её ответом.
// Веса получены скриптом examples/tinyml/train_sine.py и хранятся как int8.
#include <Arduino.h>

const int LED_PIN = 4;

// ---- НАЧАЛО СГЕНЕРИРОВАННОЙ МОДЕЛИ ----
// слой 1: 1 -> 8
const int8_t W1[8] = {
  40, 124, 100, -127, 99, 56, -52, 80
};
const float S1 = 0.01451136f;
const float BIAS1[8] = {
  0.59202f, -0.72172f, -0.37317f, 0.00000f, -0.89024f, -0.00234f, 0.00000f, -0.00330f
};
// слой 2: 8 -> 8
const int8_t W2[64] = {
  3, 36, 30, 38, -25, -9, -64, 21,
  0, -2, -41, -14, 0, -12, 102, 28,
  -118, -123, -72, -59, 7, 7, 123, -41,
  -13, 68, 22, 22, -17, -55, 6, 4,
  -45, -21, -37, -47, -3, 0, 68, -22,
  52, 42, 20, -23, 24, -17, 46, -127,
  31, -1, -42, -10, 2, 9, -33, -37,
  36, 7, 25, 37, -55, 5, 59, -49
};
const float S2 = 0.01495892f;
const float BIAS2[8] = {
  -0.26971f, 0.27132f, 0.38404f, 0.29878f, 0.00000f, -0.05366f, -1.61403f, 0.05801f
};
// слой 3: 8 -> 1
const int8_t W3[8] = {
  -127, 101, 87, 93, -12, -51, 50, -76
};
const float S3 = 0.01427681f;
const float BIAS3[1] = {
  -2.20406f
};
// всего весов: 80 (int8: 80 байт; во float было бы 320)

// ---- КОНЕЦ СГЕНЕРИРОВАННОЙ МОДЕЛИ ----

// Один полносвязный слой: out[j] = sum_i(in[i] * w[i][j]) * scale + b[j]
void dense(const float* in, int nIn, const int8_t* w, float scale,
           const float* b, float* out, int nOut, bool relu) {
  for (int j = 0; j < nOut; ++j) {
    float sum = 0;
    for (int i = 0; i < nIn; ++i) sum += in[i] * w[i * nOut + j];
    sum = sum * scale + b[j];
    out[j] = (relu && sum < 0) ? 0 : sum;
  }
}

float predictSin(float x) {                 // x в радианах, от 0 до 2π
  float in[1] = { x / (2 * PI) };           // та же нормализация, что при обучении
  float h1[8], h2[8], y[1];
  dense(in, 1, W1, S1, BIAS1, h1, 8, true);
  dense(h1, 8, W2, S2, BIAS2, h2, 8, true);
  dense(h2, 8, W3, S3, BIAS3, y, 1, false);
  return y[0];
}

void setup() {
  Serial.begin(115200);
  if (!ledcAttach(LED_PIN, 5000, 8)) { Serial.println("PWM init failed"); return; }

  // Проверка качества: максимальное отличие от настоящего sinf() на 100 точках
  float worst = 0;
  for (int k = 0; k < 100; ++k) {
    float x = 2 * PI * k / 99.0f;
    worst = max(worst, fabsf(predictSin(x) - sinf(x)));
  }
  Serial.printf("max error = %.3f\n", worst);

  // Время одного предсказания: усредняем по 1000 запусков
  volatile float sink = 0;                  // чтобы компилятор не выбросил цикл
  uint32_t t0 = micros();
  for (int k = 0; k < 1000; ++k) sink = predictSin(k * 0.00628f);
  Serial.printf("one inference = %.2f us\n", (micros() - t0) / 1000.0f);
}

void loop() {
  float x = 2 * PI * (millis() % 4000) / 4000.0f;   // один оборот за 4 секунды
  float y = predictSin(x);                          // от -1 до +1
  int duty = constrain((int)((y + 1) * 127.5f), 0, 255);
  ledcWrite(LED_PIN, duty);
  Serial.printf("x:%.2f y:%.3f\n", x, y);           // формат Serial Plotter
  delay(20);
}
