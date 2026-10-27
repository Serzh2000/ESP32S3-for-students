const int BAT_PIN = 2;                    // ADC1; делитель 100 кОм + 100 кОм
const float DIVIDER = 2.0;                // делитель уменьшает напряжение вдвое
const float LOW_BATTERY = 3.40;           // ниже -- пора выключаться

float readBattery() {                     // напряжение аккумулятора в вольтах
  uint32_t sum = 0;
  for (int i = 0; i < 32; i++) sum += analogReadMilliVolts(BAT_PIN);
  return (sum / 32.0) / 1000.0 * DIVIDER;
}

int batteryPercent(float v) {             // грубая оценка для Li-ion по напряжению
  const float V[] = {3.30, 3.60, 3.70, 3.80, 3.95, 4.20};
  const int   P[] = {0,    10,   30,   55,   80,   100};
  if (v <= V[0]) return 0;
  if (v >= V[5]) return 100;
  for (int i = 1; i < 6; i++) {
    if (v < V[i]) {                       // линейная интерполяция между точками таблицы
      return P[i - 1] + (int)((v - V[i - 1]) / (V[i] - V[i - 1]) * (P[i] - P[i - 1]));
    }
  }
  return 100;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  float v = readBattery();
  Serial.printf("Батарея: %.2f В, примерно %d %%\n", v, batteryPercent(v));
  if (v < LOW_BATTERY) {
    Serial.println("Батарея разряжена: сохраняем данные и засыпаем");
    Serial.flush();
    esp_sleep_enable_timer_wakeup(600ULL * 1000000);   // проверить снова через 10 минут
    esp_deep_sleep_start();
  }
  delay(2000);
}
