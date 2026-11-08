#include <Arduino.h>
#include "soc/gpio_struct.h"

const int LED_PIN = 4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  GPIO.out_w1ts = (1UL << LED_PIN);   // регистр «установить»: бит 4 -> вывод GPIO4 в HIGH
  delay(500);
  GPIO.out_w1tc = (1UL << LED_PIN);   // регистр «сбросить»: бит 4 -> вывод GPIO4 в LOW
  delay(500);
}
