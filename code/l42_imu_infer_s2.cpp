// Шаг 2. ФРАГМЕНТ: признаки окна на плате -- те же, что считал скетч сбора данных.
// readAccel() и настройка датчика в setup() -- как в сборе данных.
const int WINDOW = 50;
float samples[WINDOW][3];

void windowFeatures(float* feat) {           // feat[0..2] -- средние, feat[3..5] -- разброс
  for (int a = 0; a < 3; ++a) {
    feat[a] = 0;
    for (int i = 0; i < WINDOW; ++i) feat[a] += samples[i][a];
    feat[a] /= WINDOW;
    feat[3 + a] = 0;
    for (int i = 0; i < WINDOW; ++i) feat[3 + a] += sq(samples[i][a] - feat[a]);
    feat[3 + a] = sqrtf(feat[3 + a] / WINDOW);
  }
}
