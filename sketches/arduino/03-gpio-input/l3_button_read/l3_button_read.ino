const int BTN_PIN = 5;

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);   // включаем встроенную подтяжку
}

void loop() {
  bool pressed = (digitalRead(BTN_PIN) == LOW);  // pull-up: нажатие = LOW
  Serial.println(pressed ? "нажата" : "отпущена");
  delay(200);
}
