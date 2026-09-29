const int SERVO_PIN = 6;
const uint8_t RES = 14;                    // 0..16383 на период 20 мс

uint32_t usToDuty(uint32_t us) {           // микросекунды -> значение duty
  return (uint32_t)((uint64_t)us * ((1 << RES) - 1) / 20000);
}

void servoWrite(int angle) {               // 0..180 градусов
  uint32_t us = map(angle, 0, 180, 1000, 2000);  // у многих серво 500..2500
  ledcWrite(SERVO_PIN, usToDuty(us));
}

void setup() {
  ledcAttach(SERVO_PIN, 50, RES);
}

void loop() {
  for (int a = 0; a <= 180; a += 2) { servoWrite(a); delay(20); }
  for (int a = 180; a >= 0; a -= 2) { servoWrite(a); delay(20); }
}
