// Шаг 1. ФРАГМЕНТ: сеть 6-8-3. Блок весов сокращён многоточием; полный блок создаёт
// examples/tinyml/train_gesture.py, готовый скетч -- code/l42_imu_infer.cpp.

// ---- НАЧАЛО СГЕНЕРИРОВАННОЙ МОДЕЛИ ----
// классы: 0=покой, 1=тряска, 2=наклон
const int NC = 3;
// слой 1: 6 -> 8
const int8_t W1[48] = {
  41, 10, -9, -72, -14, -9, 15, -12,
  /* ... ещё 5 строк по 8 весов ... */
};
const float S1 = 0.01629021f;
const float BIAS1[8] = { 0.18936f, 0.16731f, /* ... */ 0.07001f };
// слой 2: 8 -> 3
const int8_t W2[24] = {
  92, -3, 21, -100, 18, 39, -16, -100,
  /* ... ещё 2 строки ... */
};
const float S2 = 0.01073441f;
const float BIAS2[3] = { 0.36822f, -0.36463f, 0.23739f };
// среднее и разброс признаков из обучения
const float FMEAN[6] = { 0.00933f, 0.21500f, 0.87735f, 0.16119f, 0.15984f, 0.12908f };
const float FSTD[6] = { 0.04454f, 0.34230f, 0.16016f, 0.20487f, 0.20293f, 0.15954f };
// ---- КОНЕЦ СГЕНЕРИРОВАННОЙ МОДЕЛИ ----

// dense() -- та же функция слоя, что в опыте 1.

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
