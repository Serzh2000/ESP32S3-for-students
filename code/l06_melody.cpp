// Пищалка играет гамму «до-ре-ми-фа-соль-ля-си-до»
const int BUZZER_PIN = 15;
const int NOTES[] = {262, 294, 330, 349, 392, 440, 494, 523};  // частоты, Гц

void setup() {
  ledcAttach(BUZZER_PIN, 1000, 8);
}

void loop() {
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(BUZZER_PIN, NOTES[i]);     // квадратная волна нужной частоты
    delay(300);
  }
  ledcWriteTone(BUZZER_PIN, 0);              // 0 Гц -- тишина
  delay(1500);
}
