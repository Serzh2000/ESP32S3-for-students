#include <Arduino.h>
const int IN1_PIN = 15;                   // направление (вход AIN1 драйвера)
const int IN2_PIN = 16;                   // направление (вход AIN2 драйвера)
const int PWM_PIN = 7;                    // скорость (вход PWMA драйвера)

void motor(int speed) {                   // -255..255: знак задаёт направление
  speed = constrain(speed, -255, 255);
  digitalWrite(IN1_PIN, speed > 0);
  digitalWrite(IN2_PIN, speed < 0);
  ledcWrite(PWM_PIN, abs(speed));         // 0 -- мотор останавливается
}

void setup() {
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  ledcAttach(PWM_PIN, 20000, 8);
  motor(0);
}

void loop() {
  motor(150);  delay(2000);               // вперёд, примерно 60 %
  motor(0);    delay(1000);               // остановка
  motor(-150); delay(2000);               // назад
  motor(0);    delay(1000);
}
