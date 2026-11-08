// Шаг 3. ФРАГМЕНТ: loop() -- окно, признаки, сеть и замер времени.
// Всё остальное (модель, classify(), windowFeatures(), readAccel(), setup()) -- из шагов 1 и 2.
const uint32_t PERIOD_MS = 20;
const char* NAMES[] = {"покой", "тряска", "наклон"};
int count = 0;
uint32_t nextMs = 0;

void loop() {
  if (millis() < nextMs) return;
  nextMs += PERIOD_MS;
  if (nextMs < millis()) nextMs = millis() + PERIOD_MS;

  if (!readAccel(samples[count])) { Serial.println("sensor error"); return; }
  if (++count < WINDOW) return;
  count = 0;

  float feat[6];
  windowFeatures(feat);

  uint32_t t0 = micros();
  float conf;
  int cls = classify(feat, &conf);
  uint32_t us = micros() - t0;

  Serial.printf("class:%s conf:%.2f time:%u us\n", NAMES[cls], conf, (unsigned)us);
}
