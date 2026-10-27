const int CLK_PIN = 6;     // выход A энкодера
const int DT_PIN  = 7;     // выход B энкодера
const int SW_PIN  = 15;    // кнопка на оси

volatile int position = 0;
volatile uint32_t lastEdge = 0;

void IRAM_ATTR onClk() {
  uint32_t now = micros();
  if (now - lastEdge < 2000) return;                    // дребезг: игнорируем 2 мс
  lastEdge = now;
  position += (digitalRead(DT_PIN) == HIGH) ? 1 : -1;   // DT показывает направление
}

void setup() {
  Serial.begin(115200);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK_PIN), onClk, FALLING);
}

void loop() {
  static int shown = 0;
  int p = position;
  if (p != shown) {
    shown = p;
    Serial.printf("Позиция: %d\n", p);
  }
  if (digitalRead(SW_PIN) == LOW) {                     // нажатие оси обнуляет счёт
    position = 0;
    delay(200);
  }
}
