#include <Arduino.h>
const int PIN = 4;                   // имя для номера вывода

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);              // теперь GPIO4 создаёт сигнал
}

void loop() {
  digitalWrite(PIN, HIGH);           // GPIO4: около 3,3 В относительно GND
  Serial.println("HIGH: на выводе есть сигнал");
  delay(1000);

  digitalWrite(PIN, LOW);            // GPIO4: около 0 В относительно GND
  Serial.println("LOW: сигнал выключен");
  delay(1000);
}
