// Умная лампа, шаг 7: задачи FreeRTOS
SemaphoreHandle_t lampMutex;                 // «ключ» к состоянию lamp
TaskHandle_t displayHandle;                  // дескриптор задачи экрана

LampState getLamp() {                        // безопасная копия состояния
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  LampState copy = lamp;
  xSemaphoreGive(lampMutex);
  return copy;
}

void lampSetOn(bool on) {
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  lamp.on = on;
  applyLamp();
  xSemaphoreGive(lampMutex);
}
// lampToggle() и lampSetLevel() -- по тому же образцу;
// в handleSerial() для команды status читаем getLamp() вместо lamp

void inputTask(void*) {                      // кнопка и ручка: каждые 10 мс
  int lastPot = -100;
  for (;;) {
    if (buttonPressed()) lampToggle();
    int pot = readPotLevel();
    if (abs(pot - lastPot) >= 3) { lastPot = pot; lampSetLevel(pot); }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void displayTask(void*) {                    // экран: 10 раз в секунду
  for (;;) {
    drawDisplay(getLamp());
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  Serial.begin(115200);
  lampMutex = xSemaphoreCreateMutex();       // до запуска задач!
  ledcAttach(LAMP_PIN, 5000, 8);
  pinMode(BTN_PIN, INPUT_PULLUP);
  applyLamp();
  Wire.begin(8, 9);
  Wire.setClock(400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("Дисплей не найден");
  display.setTextColor(SSD1306_WHITE);
  xTaskCreatePinnedToCore(inputTask,   "input",   4096, NULL, 2, NULL,           1);
  xTaskCreatePinnedToCore(displayTask, "display", 4096, NULL, 1, &displayHandle, 1);
}

void loop() {                                // остались только Serial и «пульс»
  handleSerial();
  heartbeat();
  delay(10);
}
