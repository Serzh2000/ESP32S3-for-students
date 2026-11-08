const int RGB_PIN = 48;                   // на ревизии v1.1 -- 38

// Перевод цвета из HSV в RGB: hue 0..359, sat и val 0..255.
void hsvToRgb(int hue, uint8_t sat, uint8_t val, uint8_t &r, uint8_t &g, uint8_t &b) {
  int region = hue / 60;                  // шесть секторов цветового круга
  int rem = (hue % 60) * 255 / 60;        // положение внутри сектора
  uint8_t p = val * (255 - sat) / 255;
  uint8_t q = val * (255 - sat * rem / 255) / 255;
  uint8_t t = val * (255 - sat * (255 - rem) / 255) / 255;
  switch (region) {
    case 0:  r = val; g = t;   b = p;   break;
    case 1:  r = q;   g = val; b = p;   break;
    case 2:  r = p;   g = val; b = t;   break;
    case 3:  r = p;   g = q;   b = val; break;
    case 4:  r = t;   g = p;   b = val; break;
    default: r = val; g = p;   b = q;   break;
  }
}

void setup() {}

void loop() {
  static int hue = 0;
  uint8_t r, g, b;
  hsvToRgb(hue, 255, 40, r, g, b);        // яркость 40 из 255: не слепит глаза
  rgbLedWrite(RGB_PIN, r, g, b);
  hue = (hue + 1) % 360;                  // за один оборот проходят все цвета радуги
  delay(15);
}
