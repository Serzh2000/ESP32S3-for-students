// Шаг 2 (урок 25): подключаем мотор. ULN2003: IN1..IN4 -> GPIO15..GPIO18.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int PINS[4] = {15, 16, 17, 18};
const uint8_t PHASES[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};
const long STEPS_PER_REV = 4096;
const uint32_t STEP_US = 1200;

std::atomic<long> requestDeg{0};
std::atomic<int> stepSize{90};
long current = 0, target = 0;             // положение и цель в шагах
uint32_t lastStep = 0;
uint16_t angleId;

void applyPhase(int phase) {
  for (int i = 0; i < 4; i++) digitalWrite(PINS[i], (PHASES[phase] >> i) & 1);
}
void release() { for (int i = 0; i < 4; i++) digitalWrite(PINS[i], LOW); }
void stepperRun() {                       // один шаг за вызов, как в основной программе урока
  if (current == target) return;
  if (micros() - lastStep < STEP_US) return;
  lastStep = micros();
  current += (target > current) ? 1 : -1;
  applyPhase(((current % 8) + 8) % 8);
  if (current == target) release();
}

void stepCallback(Control* sender, int event) { if (event == N_VALUE) stepSize = constrain(sender->value.toInt(), 1, 360); }
void plusCallback(Control*, int event) { if (event == B_UP) requestDeg += stepSize; }

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) pinMode(PINS[i], OUTPUT);
  release();
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Stepper", "lesson-39-demo")) { Serial.println("AP failed"); return; }
  ESPUI.number("Шаг поворота, °", stepCallback, ControlColor::Peterriver, 90, 1, 360);
  ESPUI.button("Вправо", plusCallback, ControlColor::Emerald, "+ шаг");
  angleId = ESPUI.label("Текущий угол, °", ControlColor::Turquoise, "0");
  ESPUI.begin("ESP32-S3 stepper");
}

void loop() {
  stepperRun();
  long deg = requestDeg.exchange(0);
  if (deg != 0) target += deg * STEPS_PER_REV / 360;

  static uint32_t last = 0;
  if (millis() - last >= 300) {           // подпись обновляем редко
    last = millis();
    ESPUI.updateLabel(angleId, String(current * 360 / STEPS_PER_REV));
  }
}
