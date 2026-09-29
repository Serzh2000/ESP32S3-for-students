void blinkTask(void* param) {
  int pin = (int)(intptr_t)param;          // номер вывода передан параметром
  int period = (pin == 4) ? 300 : 700;
  pinMode(pin, OUTPUT);
  for (;;) {                               // задача никогда не завершается сама
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(period));     // отдаём процессор другим задачам
  }
}

void setup() {
  Serial.begin(115200);
  //                     функция    имя   стек  параметр        приор. дескр. ядро
  xTaskCreatePinnedToCore(blinkTask, "b1", 2048, (void*)(intptr_t)4, 1, NULL, 0);
  xTaskCreatePinnedToCore(blinkTask, "b2", 2048, (void*)(intptr_t)7, 1, NULL, 1);
}

void loop() {
  Serial.printf("loop() на ядре %d\n", xPortGetCoreID());
  delay(2000);
}
