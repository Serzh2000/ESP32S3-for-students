#include <Arduino.h>
// Шаг 4 (итог). Добавляется предел времени одного пуска: если связь пропала, мотор остановится сам.
// Драйвер TB6612: AIN1 -> GPIO15, AIN2 -> GPIO16, PWMA -> GPIO7, STBY -> 3V3 (урок 16).
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int IN1_PIN = 15;
const int IN2_PIN = 16;
const int PWM_PIN = 7;
const uint32_t MAX_RUN_MS = 4000;         // предел одного непрерывного пуска: страховка при потере связи

std::atomic<int> direction{0};            // -1 назад, 0 стоп, +1 вперёд
std::atomic<int> speedPercent{40};
std::atomic<uint32_t> runStart{0};
uint16_t stateId;

void motor(int speed) {                   // -255..255, как в листинге урока
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
    case P_FOR_DOWN:  direction = 1;  runStart = millis(); break;
    case P_BACK_DOWN: direction = -1; runStart = millis(); break;
    case P_FOR_UP:                    // отпускание кнопки = остановка
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
  static int applied = 0;
  int dir = direction;
  if (dir != 0 && millis() - runStart > MAX_RUN_MS) {     // время вышло: остановка
    direction = 0;
    dir = 0;
  }
  int target = dir * speedPercent * 255 / 100;
  if (target != applied) {
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
