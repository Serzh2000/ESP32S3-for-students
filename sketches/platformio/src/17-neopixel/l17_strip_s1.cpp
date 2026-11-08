#include <Arduino.h>
// Шаг 1. Радуга на ленте WS2812 из 8 пикселей: один шаг анимации каждые 40 мс, без delay().
#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 16;                  // через резистор 330 Ом
const int NUM_PIXELS = 8;
const uint8_t BRIGHTNESS = 40;            // 0..255; ограничивает ток всей ленты

Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t lastStep = 0;
uint16_t hueOffset = 0;                   // сдвиг радуги, 0..65535

void rainbowStep() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    uint16_t hue = hueOffset + i * 65536UL / NUM_PIXELS;
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
  hueOffset += 256;
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();                           // сразу погасить все пиксели
}

void loop() {
  uint32_t now = millis();
  if (now - lastStep >= 40) {             // шаг анимации без delay()
    lastStep = now;
    rainbowStep();
    strip.show();
  }
}
