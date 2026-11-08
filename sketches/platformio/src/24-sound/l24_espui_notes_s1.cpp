#include <Arduino.h>
// Шаг 1 (урок 24): выбор ноты в списке на телефоне. Пищалка пока молчит.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

const uint16_t NOTES[] = {262, 294, 330, 349, 392, 440, 494, 523};   // до ... до

std::atomic<int> noteIndex{0};            // выбор из списка: пишет обработчик
uint16_t freqId;

void noteCallback(Control* sender, int event) {
  if (event != S_VALUE) return;
  int i = sender->value.toInt();          // в value хранится номер ноты как строка
  if (i >= 0 && i < 8) noteIndex = i;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Notes", "lesson-39-demo")) { Serial.println("AP failed"); return; }

  auto select = ESPUI.addControl(ControlType::Select, "Нота", "0", ControlColor::Sunflower, Control::noParent, noteCallback);
  const char* names[] = {"до (C4)", "ре (D4)", "ми (E4)", "фа (F4)", "соль (G4)", "ля (A4)", "си (B4)", "до (C5)"};
  for (int i = 0; i < 8; i++)
    ESPUI.addControl(ControlType::Option, names[i], String(i), ControlColor::None, select);
  freqId = ESPUI.label("Частота, Гц", ControlColor::Peterriver, String(NOTES[0]));
  ESPUI.begin("ESP32-S3 notes");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static int shown = -1;
  int i = noteIndex;
  if (i != shown) {                       // подпись обновляем только при смене ноты
    shown = i;
    ESPUI.updateLabel(freqId, String(NOTES[i]));
  }
  delay(20);
}
