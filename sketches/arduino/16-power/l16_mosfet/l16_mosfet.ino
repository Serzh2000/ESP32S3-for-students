const int GATE_PIN = 7;                   // затвор N-MOSFET через резистор 100 Ом

void setup() {
  ledcAttach(GATE_PIN, 20000, 8);         // 20 кГц: выше слышимого диапазона
}

void loop() {
  for (int d = 0; d <= 255; d += 5) {     // плавный разгон нагрузки
    ledcWrite(GATE_PIN, d);
    delay(30);
  }
  for (int d = 255; d >= 0; d -= 5) {     // и плавное затухание
    ledcWrite(GATE_PIN, d);
    delay(30);
  }
}
