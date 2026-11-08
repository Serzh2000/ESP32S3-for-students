#include <Arduino.h>
#include "SD_MMC.h"

// Слот microSD (ESP32-S3-EYE): CLK 39, CMD 38, D0 40. Сверьте со своей платой!
const int SD_CLK = 39;
const int SD_CMD = 38;
const int SD_D0  = 40;

void setup() {
  Serial.begin(115200);
  delay(1000);
  SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0);  // режим с одной линией данных (1-bit)
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("microSD не найдена: проверьте карту (FAT32) и выводы слота");
    return;
  }
  File f = SD_MMC.open("/test.txt", FILE_WRITE);
  if (f) {
    f.println("Hello, card!");
    f.close();
    Serial.println("Файл /test.txt записан");
  }
}

void loop() {}
