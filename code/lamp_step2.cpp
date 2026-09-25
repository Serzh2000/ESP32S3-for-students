// Умная лампа, шаг 2: управление из Serial Monitor
const int LAMP_PIN = 4;
const int RGB_PIN  = 48;          // на ревизии v1.1 -- 38

bool lampOn = false;

void setLamp(bool on) {
  lampOn = on;
  digitalWrite(LAMP_PIN, lampOn);
}

void handleSerial() {
  if (!Serial.available()) return;           // команд нет -- не ждём
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();                                // убрать \r и пробелы
  if (cmd == "on")          { setLamp(true);  Serial.println("OK"); }
  else if (cmd == "off")    { setLamp(false); Serial.println("OK"); }
  else if (cmd == "status") Serial.printf("lamp=%s, uptime=%lu с\n",
                                          lampOn ? "on" : "off", millis() / 1000);
  else                      Serial.println("Команды: on, off, status");
}

void heartbeat() {                           // из шага 1
  static uint32_t last = 0;
  static bool beat = false;
  if (millis() - last >= 500) {
    last = millis();
    beat = !beat;
    rgbLedWrite(RGB_PIN, 0, beat ? 20 : 0, 0);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LAMP_PIN, OUTPUT);
  setLamp(false);
}

void loop() {
  handleSerial();
  heartbeat();
}
