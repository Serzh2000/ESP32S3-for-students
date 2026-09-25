#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);   // -1: без вывода сброса

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  Wire.setClock(400000);                        // Fast mode, 400 кГц
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Дисплей не найден");
    while (true) delay(1000);
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  display.clearDisplay();                       // очистить буфер
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("ESP32-S3");
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.printf("uptime: %lu s", millis() / 1000);
  display.display();                            // отправить буфер на экран
  delay(200);
}
