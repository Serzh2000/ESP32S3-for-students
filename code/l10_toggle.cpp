// Кнопка переключает яркость: выкл -> 30 % -> 100 % -> выкл ...
const int LED_PIN = 4;
const int BTN_PIN = 5;
const int LEVELS[] = {0, 30, 100};           // ступени яркости, %

int mode = 0;                                // номер текущей ступени

uint32_t gammaDuty(int percent) {
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

bool buttonPressed() {                       // см. выше: с защитой от дребезга
  static bool stable = digitalRead(BTN_PIN);
  static bool last = stable;
  static uint32_t changed = 0;
  bool r = digitalRead(BTN_PIN);
  if (r != last) { last = r; changed = millis(); }
  if (millis() - changed > 50 && r != stable) {
    stable = r;
    return stable == LOW;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  ledcAttach(LED_PIN, 5000, 8);
  ledcWrite(LED_PIN, 0);
}

void loop() {
  if (buttonPressed()) {
    mode = (mode + 1) % 3;                   // 0 -> 1 -> 2 -> 0
    ledcWrite(LED_PIN, gammaDuty(LEVELS[mode]));
    Serial.printf("Яркость: %d%%\n", LEVELS[mode]);
  }
}
