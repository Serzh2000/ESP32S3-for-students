void setup() {
  Serial.begin(115200);             // открыть канал связи с компьютером
  delay(500);                       // дать USB время подняться
  Serial.println("Привет, ESP32-S3!");
  Serial.printf("Частота CPU: %lu МГц\n", getCpuFrequencyMhz());
  Serial.printf("Свободно SRAM: %lu байт\n", ESP.getFreeHeap());
  Serial.printf("PSRAM: %lu байт\n", ESP.getPsramSize());
  Serial.printf("Flash: %lu байт\n", ESP.getFlashChipSize());
}

void loop() {
  Serial.printf("Прошло %lu мс\n", millis());
  delay(1000);
}
