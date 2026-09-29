QueueHandle_t q;

void sensorTask(void*) {
  for (;;) {
    int mv = analogReadMilliVolts(1);
    xQueueSend(q, &mv, 0);                      // не ждать, если очередь полна
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void printTask(void*) {
  int mv;
  for (;;) {
    if (xQueueReceive(q, &mv, portMAX_DELAY) == pdTRUE) {  // спим до прихода данных
      Serial.printf("U = %d мВ\n", mv);
    }
  }
}

void setup() {
  Serial.begin(115200);
  q = xQueueCreate(5, sizeof(int));
  xTaskCreatePinnedToCore(sensorTask, "sensor", 3072, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(printTask,  "print",  4096, NULL, 1, NULL, 1);
}

void loop() { vTaskDelete(NULL); }              // loopTask больше не нужен
