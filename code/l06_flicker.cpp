// Опыт: с какой частоты глаз перестаёт видеть мигание?
const int LED_PIN = 4;
int halfPeriod = 500;                        // половина периода, мс

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < 6; i++) {              // мигнём 6 раз с текущей частотой
    digitalWrite(LED_PIN, HIGH);
    delay(halfPeriod);
    digitalWrite(LED_PIN, LOW);
    delay(halfPeriod);
  }
  halfPeriod = halfPeriod / 2;               // в два раза быстрее
  if (halfPeriod < 2) halfPeriod = 500;      // и всё сначала
  Serial.printf("Частота: %.1f Гц\n", 1000.0 / (2 * halfPeriod));
}
