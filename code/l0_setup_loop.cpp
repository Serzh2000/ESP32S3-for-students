void setup() {
  Serial.begin(115200);              // открыть связь с компьютером
  Serial.println("Старт: setup() был вызван один раз");
}

void loop() {
  Serial.println("Сейчас выполняется loop()");
  delay(500);                        // подождать полсекунды
}
