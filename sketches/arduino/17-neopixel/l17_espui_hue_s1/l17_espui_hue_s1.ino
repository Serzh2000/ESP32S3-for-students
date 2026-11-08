// Шаг 1. Без сети: перевод HSV -> RGB и плавная смена оттенка на встроенном светодиоде (GPIO48; на v1.1 --- GPIO38).
const int RGB_PIN = 48;

void hsvToRgb(int hue, uint8_t sat, uint8_t val, uint8_t &r, uint8_t &g, uint8_t &b) {
  int region = hue / 60;                  // какой из шести секторов цветового круга
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

void setup() {
  Serial.begin(115200);
}

void loop() {
  static int hue = 0;                     // 0..359
  uint8_t r, g, b;
  hsvToRgb(hue, 255, 40, r, g, b);        // яркость 40 из 255: для глаз достаточно
  rgbLedWrite(RGB_PIN, r, g, b);
  hue = (hue + 1) % 360;
  delay(20);
}
