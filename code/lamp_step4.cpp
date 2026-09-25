// Умная лампа, шаг 4: плавная яркость через ШИМ
struct LampState {
  bool on;
  int  level;                                // яркость 0..100 %
};
LampState lamp = {false, 50};                // заменяет bool lampOn

uint32_t levelToDuty(int level) {            // гамма-коррекция
  return (uint32_t)(255 * pow(level / 100.0, 2.2) + 0.5);
}

void applyLamp() {                           // состояние -> вывод
  ledcWrite(LAMP_PIN, lamp.on ? levelToDuty(lamp.level) : 0);
}

void lampSetOn(bool on)      { lamp.on = on;                         applyLamp(); }
void lampToggle()            { lamp.on = !lamp.on;                   applyLamp(); }
void lampSetLevel(int level) { lamp.level = constrain(level, 0, 100); applyLamp(); }

void handleSerial() {                        // + команда level N
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  if (cmd == "on")                   { lampSetOn(true);  Serial.println("OK"); }
  else if (cmd == "off")             { lampSetOn(false); Serial.println("OK"); }
  else if (cmd.startsWith("level ")) { lampSetLevel(cmd.substring(6).toInt()); Serial.println("OK"); }
  else if (cmd == "status")          Serial.printf("lamp=%s level=%d%%\n",
                                                   lamp.on ? "on" : "off", lamp.level);
  else                               Serial.println("Команды: on, off, level N, status");
}

// heartbeat(), buttonPressed() -- без изменений

void setup() {
  Serial.begin(115200);
  ledcAttach(LAMP_PIN, 5000, 8);             // вместо pinMode(LAMP_PIN, OUTPUT)
  pinMode(BTN_PIN, INPUT_PULLUP);
  applyLamp();
}

void loop() {
  handleSerial();
  if (buttonPressed()) lampToggle();
  heartbeat();
}
