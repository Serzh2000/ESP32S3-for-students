#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 16;                  // через резистор 330 Ом
const int NUM_PIXELS = 8;
const uint8_t BRIGHTNESS = 40;            // 0..255; ограничивает ток всей ленты

Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t lastStep = 0;
uint32_t lastSwitch = 0;
uint16_t hueOffset = 0;                   // сдвиг радуги, 0..65535
int head = 0;                             // положение «бегущего огня»
bool rainbowMode = true;

void rainbowStep() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    uint16_t hue = hueOffset + i * 65536UL / NUM_PIXELS;
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
  hueOffset += 256;
}

void chaseStep() {
  strip.clear();
  strip.setPixelColor(head, strip.Color(255, 60, 0));  // оранжевая «искра»
  head = (head + 1) % NUM_PIXELS;
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();                           // сразу погасить все пиксели
}

void loop() {
  uint32_t now = millis();
  if (now - lastSwitch >= 8000) {         // каждые 8 с меняем эффект
    lastSwitch = now;
    rainbowMode = !rainbowMode;
  }
  if (now - lastStep >= 40) {             // шаг анимации без delay()
    lastStep = now;
    if (rainbowMode) rainbowStep(); else chaseStep();
    strip.show();
  }
}
