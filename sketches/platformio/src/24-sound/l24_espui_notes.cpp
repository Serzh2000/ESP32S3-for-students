#include <Arduino.h>
// Пассивная пищалка между GPIO21 и GND (урок 24). Выбираем ноту и нажимаем «Играть».
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const int BUZZER_PIN = 21;
const uint16_t NOTES[] = {262, 294, 330, 349, 392, 440, 494, 523};   // до ... до
const uint32_t BEEP_MS = 400;

std::atomic<int> noteIndex{0};
std::atomic<bool> playRequest{false};
uint16_t freqId;

void noteCallback(Control* sender, int event) {
  if (event != S_VALUE) return;
  int i = sender->value.toInt();          // в value хранится номер ноты как строка
  if (i >= 0 && i < 8) noteIndex = i;
}

void playCallback(Control*, int event) {
  if (event == B_UP) playRequest = true;  // один раз на щелчок
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Notes", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  auto select = ESPUI.addControl(ControlType::Select, "Нота", "0", ControlColor::Sunflower, Control::noParent, noteCallback);
  const char* names[] = {"до (C4)", "ре (D4)", "ми (E4)", "фа (F4)", "соль (G4)", "ля (A4)", "си (B4)", "до (C5)"};
  for (int i = 0; i < 8; i++)
    ESPUI.addControl(ControlType::Option, names[i], String(i), ControlColor::None, select);
  ESPUI.button("Звук", playCallback, ControlColor::Emerald, "Играть");
  freqId = ESPUI.label("Частота, Гц", ControlColor::Peterriver, String(NOTES[0]));
  ESPUI.begin("ESP32-S3 notes");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t stopAt = 0;
  if (playRequest.exchange(false)) {
    uint16_t f = NOTES[noteIndex];
    tone(BUZZER_PIN, f);
    stopAt = millis() + BEEP_MS;          // выключим позже, не блокируя программу
    ESPUI.updateLabel(freqId, String(f));
  }
  if (stopAt && (int32_t)(millis() - stopAt) >= 0) {
    noTone(BUZZER_PIN);
    stopAt = 0;
  }
  delay(2);
}
