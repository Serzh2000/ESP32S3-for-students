// Умная лампа, шаг 3: кнопка-выключатель
const int BTN_PIN = 5;
const uint32_t DEBOUNCE_MS = 50;

// setLamp(), handleSerial(), heartbeat() -- без изменений (шаг 2)
// buttonPressed() -- из раздела 3.3

void setup() {
  Serial.begin(115200);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  setLamp(false);
}

void loop() {
  handleSerial();
  if (buttonPressed()) setLamp(!lampOn);   // нажатие переключает лампу
  heartbeat();
}
