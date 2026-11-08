#include <Arduino.h>
// Шаг 3. Безопасный реверс: перед сменой направления короткая остановка. Состояние видно в подписи.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int IN1_PIN = 15;
const int IN2_PIN = 16;
const int PWM_PIN = 7;

std::atomic<int> direction{0};            // -1 назад, 0 стоп, +1 вперёд
std::atomic<int> speedPercent{40};
uint16_t stateId;                         // новое: номер подписи состояния

void motor(int speed) {                   // -255..255
  speed = constrain(speed, -255, 255);
  digitalWrite(IN1_PIN, speed > 0);
  digitalWrite(IN2_PIN, speed < 0);
  ledcWrite(PWM_PIN, abs(speed));
}

void speedCallback(Control* sender, int event) {
  if (event == SL_VALUE) speedPercent = constrain(sender->value.toInt(), 0, 100);
}

void padCallback(Control*, int event) {
  switch (event) {
    case P_FOR_DOWN:  direction = 1;  break;
    case P_BACK_DOWN: direction = -1; break;
    case P_FOR_UP:
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

  ESPUI.sliderContinuous = false;
  ESPUI.slider("Скорость, %", speedCallback, ControlColor::Peterriver, 40, 0, 100);
  ESPUI.pad("Вперёд / назад (держите)", padCallback, ControlColor::Carrot);
  stateId = ESPUI.label("Применённое состояние", ControlColor::Turquoise, "СТОП");
  ESPUI.begin("ESP32-S3 motor");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static int applied = 0;                 // что сейчас подано на мотор
  int dir = direction;
  int target = dir * speedPercent * 255 / 100;
  if (target != applied) {                // меняем мотор только при изменении
    if ((target > 0) != (applied > 0) && applied != 0) {   // реверс: сначала пауза (урок 16)
      motor(0);
      delay(200);
    }
    motor(target);
    applied = target;
    ESPUI.updateLabel(stateId, dir == 0 ? "СТОП" : (dir > 0 ? "ВПЕРЁД" : "НАЗАД"));
  }
  delay(5);
}
