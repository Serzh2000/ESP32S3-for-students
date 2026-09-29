// Вольтметр с экраном: напряжение на ручке крупно и полоска-шкала
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int POT_PIN = 1;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

int readMilliVolts() {
  uint32_t sum = 0;
  for (int i = 0; i < 16; i++) sum += analogReadMilliVolts(POT_PIN);
  return sum / 16;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);                          // SDA, SCL
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Дисплей не найден");
    while (true) delay(1000);
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int mv = readMilliVolts();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Voltmeter");
  display.setTextSize(3);
  display.setCursor(0, 18);
  display.printf("%.2fV", mv / 1000.0);
  display.drawRect(0, 54, 128, 10, SSD1306_WHITE);        // рамка шкалы
  display.fillRect(2, 56, mv * 124 / 3300, 6, SSD1306_WHITE);
  display.display();                         // отправить картинку на экран
  delay(100);                                // 10 кадров в секунду хватит
}
