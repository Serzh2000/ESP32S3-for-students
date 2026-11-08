// Шаг 1. Pad «вперёд/назад»: мотор едет, пока кнопка нажата. Скорость пока фиксированная, 40 %.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int IN1_PIN = 15;
const int IN2_PIN = 16;
const int PWM_PIN = 7;

std::atomic<int> direction{0};            // -1 назад, 0 стоп, +1 вперёд

void motor(int speed) {                   // -255..255, как в листинге урока
  speed = constrain(speed, -255, 255);
  digitalWrite(IN1_PIN, speed > 0);
  digitalWrite(IN2_PIN, speed < 0);
  ledcWrite(PWM_PIN, abs(speed));
}

void padCallback(Control*, int event) {   // обработчик только запоминает желание
  switch (event) {
    case P_FOR_DOWN:  direction = 1;  break;
    case P_BACK_DOWN: direction = -1; break;
    case P_FOR_UP:                        // отпускание кнопки = остановка
    case P_BACK_UP:
    case P_CENTER_DOWN: direction = 0; break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  if (!ledcAttach(PWM_PIN, 20000, 8)) { Serial.println("PWM init failed"); return; }
  motor(0);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Motor", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  ESPUI.pad("Вперёд / назад (держите)", padCallback, ControlColor::Carrot);
  ESPUI.begin("ESP32-S3 motor");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  motor(direction * 40 * 255 / 100);      // мотором управляет только loop()
  delay(5);
}
