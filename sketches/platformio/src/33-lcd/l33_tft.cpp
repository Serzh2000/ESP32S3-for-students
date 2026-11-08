#include <Arduino.h>
// 240x240 SPI ST7789 module with CS pin and 3.3V logic.
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
Adafruit_ST7789 screen(10, 9, 8); // CS, DC, RST
void setup() {
  Serial.begin(115200);
  SPI.begin(12, -1, 11, 10); // SCK, MISO unused, MOSI, CS
  screen.init(240, 240); screen.setRotation(0);
  screen.fillScreen(ST77XX_BLACK);
  screen.setTextColor(ST77XX_WHITE); screen.setTextSize(2);
  screen.setCursor(12, 20); screen.print("ESP32-S3");
  screen.drawRect(19, 99, 202, 22, ST77XX_WHITE);
}
void loop() {
  static uint32_t last = 0;
  static int oldWidth = 0;
  if (millis() - last < 100) return;
  last = millis();
  int width = analogRead(1) * 200L / 4095;
  uint32_t start = micros();
  if (width > oldWidth)
    screen.fillRect(20 + oldWidth, 100, width - oldWidth, 20, ST77XX_GREEN);
  if (width < oldWidth)
    screen.fillRect(20 + width, 100, oldWidth - width, 20, ST77XX_BLACK);
  oldWidth = width;
  Serial.printf("draw_us:%lu\n", (unsigned long)(micros() - start));
}
