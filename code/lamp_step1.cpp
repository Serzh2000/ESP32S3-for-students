// Умная лампа, шаг 1: светодиод лампы и «пульс» на RGB
const int LAMP_PIN = 4;
const int RGB_PIN  = 48;          // на ревизии v1.1 -- 38

void heartbeat() {                // мигает раз в секунду, не блокируя loop()
  static uint32_t last = 0;
  static bool beat = false;
  if (millis() - last >= 500) {
    last = millis();
    beat = !beat;
    rgbLedWrite(RGB_PIN, 0, beat ? 20 : 0, 0);
  }
}

void setup() {
  pinMode(LAMP_PIN, OUTPUT);
  digitalWrite(LAMP_PIN, HIGH);   // пока просто включаем лампу
}

void loop() {
  heartbeat();
}
