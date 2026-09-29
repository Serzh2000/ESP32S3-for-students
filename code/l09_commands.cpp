// Управляем светодиодом командами из Serial Monitor
const int LED_PIN = 4;

bool ledOn = false;
int level = 50;                              // яркость, 0..100 %

uint32_t gammaDuty(int percent) {            // из урока про ШИМ
  return (uint32_t)(255 * pow(percent / 100.0, 2.2) + 0.5);
}

void applyLed() {                            // показать состояние на светодиоде
  ledcWrite(LED_PIN, ledOn ? gammaDuty(level) : 0);
}

void handleSerial() {
  if (!Serial.available()) return;           // команд нет -- не ждём
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();                                // убрать лишние пробелы и \r
  if (cmd == "on") {
    ledOn = true;
  } else if (cmd == "off") {
    ledOn = false;
  } else if (cmd.startsWith("level ")) {     // например, "level 30"
    level = constrain(cmd.substring(6).toInt(), 0, 100);
  } else if (cmd == "status") {
    Serial.printf("светодиод: %s, яркость %d%%\n", ledOn ? "вкл" : "выкл", level);
    return;
  } else {
    Serial.println("Команды: on, off, level N, status");
    return;
  }
  applyLed();
  Serial.println("OK");
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);
  applyLed();
}

void loop() {
  handleSerial();
  // здесь может быть что-то ещё: handleSerial() ничего не ждёт
}
