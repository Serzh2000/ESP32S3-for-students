// Шаг 3. ФРАГМЕНТ для книги: сеть 1-8-8-1. Блок весов сокращён многоточием;
// полный блок создаёт examples/tinyml/train_sine.py, готовый скетч -- code/l42_sine_nn.cpp.

// ---- НАЧАЛО СГЕНЕРИРОВАННОЙ МОДЕЛИ ----
// слой 1: 1 -> 8
const int8_t W1[8] = { 40, 124, 100, -127, 99, 56, -52, 80 };
const float S1 = 0.01451136f;
const float BIAS1[8] = { 0.59202f, -0.72172f, -0.37317f, /* ... */ -0.00330f };
// слой 2: 8 -> 8
const int8_t W2[64] = {
  3, 36, 30, 38, -25, -9, -64, 21,
  /* ... ещё 7 строк по 8 весов ... */
};
const float S2 = 0.01495892f;
const float BIAS2[8] = { -0.26971f, 0.27132f, /* ... */ 0.05801f };
// слой 3: 8 -> 1
const int8_t W3[8] = { -127, 101, 87, 93, -12, -51, 50, -76 };
const float S3 = 0.01427681f;
const float BIAS3[1] = { -2.20406f };
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
