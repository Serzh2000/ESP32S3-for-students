void setup() {
  Serial.begin(115200);                          // к компьютеру
  Serial1.begin(9600, SERIAL_8N1, 18, 17);       // RX=18, TX=17 к модулю
}

void loop() {
  while (Serial1.available()) Serial.write(Serial1.read());
  while (Serial.available())  Serial1.write(Serial.read());
}
