const int RGB_PIN = 48;   // на ревизии v1.1 -- 38

void setup() {}

void loop() {
  rgbLedWrite(RGB_PIN, 40, 0, 0);  delay(400);  // красный (яркость 0..255)
  rgbLedWrite(RGB_PIN, 0, 40, 0);  delay(400);  // зелёный
  rgbLedWrite(RGB_PIN, 0, 0, 40);  delay(400);  // синий
  rgbLedWrite(RGB_PIN, 0, 0, 0);   delay(400);  // выключен
}
