// Шаг 5. ФРАГМЕНТ: loop() -- ответ сети управляет яркостью светодиода.
// В setup() добавьте: ledcAttach(LED_PIN, 5000, 8);  и вызов selfTest();
void loop() {
  float x = 2 * PI * (millis() % 4000) / 4000.0f;   // один оборот за 4 секунды
  float y = predictSin(x);                          // от -1 до +1
  int duty = constrain((int)((y + 1) * 127.5f), 0, 255);
  ledcWrite(LED_PIN, duty);
  Serial.printf("x:%.2f y:%.3f\n", x, y);           // формат Serial Plotter
  delay(20);
}
