#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin(8, 9);                  // SDA, SCL
  Serial.println("Сканирование...");
  int found = 0;
  for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {   // 0 = устройство ответило ACK
      Serial.printf("  найдено устройство: 0x%02X\n", addr);
      found++;
    }
  }
  Serial.printf("Всего: %d\n", found);
}

void loop() {}
