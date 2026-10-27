struct Sensor { int pin; int value; };
Sensor* sensor = nullptr;               // ошибка: объект так и не создан

int readSensor(Sensor* s) {
  s->value = analogRead(s->pin);        // s == nullptr -> LoadProhibited
  return s->value;
}

void setup() { Serial.begin(115200); }

void loop() {
  Serial.println(readSensor(sensor));
  delay(1000);
}
