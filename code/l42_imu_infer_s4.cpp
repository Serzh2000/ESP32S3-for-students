// Шаг 4. ФРАГМЕНТ: порог уверенности и подтверждение. Заменяет loop() из шага 3.
const float MIN_CONFIDENCE = 0.80f;         // ниже -- "не уверен", класс не объявляем
const int CONFIRM = 3;                      // сколько окон подряд должны согласиться

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

  static int lastCls = -1, streak = 0;
  int shown = (conf >= MIN_CONFIDENCE) ? cls : -1;      // -1 -- «неизвестно»
  streak = (shown == lastCls) ? streak + 1 : 1;
  lastCls = shown;

  Serial.printf("class:%s conf:%.2f time:%u us%s\n",
                shown < 0 ? "?" : NAMES[shown], conf, (unsigned)us,
                (shown >= 0 && streak >= CONFIRM) ? "  <-- ПОДТВЕРЖДЕНО" : "");
}
