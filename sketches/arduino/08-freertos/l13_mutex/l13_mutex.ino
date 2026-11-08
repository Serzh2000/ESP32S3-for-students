// Две задачи печатают в Serial. Мьютекс не даёт им перебивать друг друга
SemaphoreHandle_t serialMutex;               // «ключ» от Serial
const bool USE_MUTEX = true;                 // поставь false и сравни!

void say(const char* text) {
  if (USE_MUTEX) xSemaphoreTake(serialMutex, portMAX_DELAY);  // взять ключ
  for (const char* c = text; *c; c++) {      // печатаем по одной букве,
    Serial.print(*c);                        // как будто это долгая работа
    delayMicroseconds(300);
  }
  Serial.println();
  if (USE_MUTEX) xSemaphoreGive(serialMutex);                 // вернуть ключ
}

void catTask(void*) {
  for (;;) { say("Кошка говорит: мяу-мяу-мяу"); vTaskDelay(pdMS_TO_TICKS(7)); }
}

void dogTask(void*) {
  for (;;) { say("Собака говорит: гав-гав-гав"); vTaskDelay(pdMS_TO_TICKS(5)); }
}

void setup() {
  Serial.begin(115200);
  serialMutex = xSemaphoreCreateMutex();     // создать до запуска задач!
  xTaskCreatePinnedToCore(catTask, "cat", 3072, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(dogTask, "dog", 3072, NULL, 1, NULL, 1);
}

void loop() { vTaskDelete(NULL); }           // loop() нам не нужен
