// Ручка-потенциометр поворачивает сервопривод
const int POT_PIN   = 1;                     // ADC1
const int SERVO_PIN = 6;
const uint8_t RES = 14;                      // 0..16383 на период 20 мс

void servoWrite(int angle) {                 // 0..180 градусов
  uint32_t us = map(angle, 0, 180, 1000, 2000);        // длина импульса, мкс
  ledcWrite(SERVO_PIN, us * ((1 << RES) - 1) / 20000);
}

int readPot() {                              // среднее из 8 измерений, мВ
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) sum += analogReadMilliVolts(POT_PIN);
  return sum / 8;
}

int lastAngle = -100;

void setup() {
  Serial.begin(115200);
  ledcAttach(SERVO_PIN, 50, RES);            // 50 Гц -- как любит серво
}

void loop() {
  int angle = constrain(map(readPot(), 0, 3100, 0, 180), 0, 180);
  if (abs(angle - lastAngle) >= 2) {         // не дёргаемся из-за шума
    lastAngle = angle;
    servoWrite(angle);
    Serial.printf("угол: %d\n", angle);
  }
  delay(20);
}
