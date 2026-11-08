const int POT_PIN = 1;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);                   // 0..4095
}

void loop() {
  int raw = analogRead(POT_PIN);
  int mv  = analogReadMilliVolts(POT_PIN);    // с учётом калибровки
  Serial.printf("raw:%d mV:%d\n", raw, mv);   // формат для Serial Plotter
  delay(100);
}
