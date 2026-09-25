// Умная лампа, шаг 6: экран состояния
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void drawDisplay(const LampState& s) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Smart lamp");
  display.setTextSize(3);
  display.setCursor(0, 18);
  if (s.on) display.printf("%3d%%", s.level);
  else      display.print("OFF");
  display.drawRect(0, 54, 128, 10, SSD1306_WHITE);           // рамка полоски
  if (s.on) display.fillRect(2, 56, s.level * 124 / 100, 6, SSD1306_WHITE);
  display.display();
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LAMP_PIN, 5000, 8);
  pinMode(BTN_PIN, INPUT_PULLUP);
  applyLamp();
  Wire.begin(8, 9);
  Wire.setClock(400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("Дисплей не найден");
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  handleSerial();
  if (buttonPressed()) lampToggle();
  int pot = readPotLevel();
  if (abs(pot - lastPot) >= 3) { lastPot = pot; lampSetLevel(pot); }
  heartbeat();

  static uint32_t lastDraw = 0;                 // экран -- 10 раз в секунду
  if (millis() - lastDraw >= 100) {
    lastDraw = millis();
    drawDisplay(lamp);
  }
}
