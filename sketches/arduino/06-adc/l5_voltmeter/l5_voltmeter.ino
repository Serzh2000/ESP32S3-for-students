const int VIN_PIN = 2;
const float K = (100.0 + 33.0) / 33.0;     // коэффициент делителя

float readVoltage() {
  uint32_t sum = 0;
  for (int i = 0; i < 32; i++) sum += analogReadMilliVolts(VIN_PIN);
  return (sum / 32.0) / 1000.0 * K;        // вольты на входе делителя
}

void setup() { Serial.begin(115200); }

void loop() {
  Serial.printf("U = %.2f В\n", readVoltage());
  delay(500);
}
