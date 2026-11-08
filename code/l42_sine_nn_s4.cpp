// Шаг 4. ФРАГМЕНТ: самопроверка сети. Блок модели, dense() и predictSin() -- из шага 3;
// функцию вызывают из setup() после Serial.begin(115200).
void selfTest() {
  // Качество: максимальное отличие от настоящего sinf() на 100 точках
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
