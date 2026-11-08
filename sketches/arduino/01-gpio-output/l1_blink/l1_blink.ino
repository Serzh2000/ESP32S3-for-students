const int LED_PIN = 4;

void setup() {
  pinMode(LED_PIN, OUTPUT);     // настроить вывод как выход
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // 3,3 В -> светодиод горит
  delay(500);
  digitalWrite(LED_PIN, LOW);   // 0 В -> гаснет
  delay(500);
}
