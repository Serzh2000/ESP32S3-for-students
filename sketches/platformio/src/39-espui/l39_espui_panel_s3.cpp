#include <Arduino.h>
// Шаг 3. Ползунок яркости: строка проверяется до того, как попадёт в очередь.
// LED: GPIO4 -> 220 ohm -> LED -> GND.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

enum class Kind : uint8_t { Power, Level };
struct Command { Kind kind; int value; };
QueueHandle_t commands;
std::atomic<unsigned> dropped{0};
bool on = false;
int level = 40;                           // яркость 0..100 %

void enqueue(Kind kind, int value) {
  Command cmd{kind, value};
  if (xQueueSend(commands, &cmd, 0) != pdTRUE) ++dropped;
}

int parsePercent(const String& text) {    // -1, если это не число 0..100
  if (text.length() == 0 || text.length() > 3) return -1;
  for (size_t i = 0; i < text.length(); ++i)
    if (text[i] < '0' || text[i] > '9') return -1;
  int value = text.toInt();
  return value <= 100 ? value : -1;
}

void powerCallback(Control*, int event) {
  if (event == S_ACTIVE)   enqueue(Kind::Power, 1);
  if (event == S_INACTIVE) enqueue(Kind::Power, 0);
}
void levelCallback(Control* sender, int event) {
  if (event != SL_VALUE) return;
  int value = parsePercent(sender->value);
  if (value >= 0) enqueue(Kind::Level, value);
}

void setup() {
  Serial.begin(115200);
  commands = xQueueCreate(12, sizeof(Command));
  ledcAttach(4, 5000, 8);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-39-demo");
  ESPUI.sliderContinuous = false;
  auto mainTab = ESPUI.addControl(ControlType::Tab, "", "Control");
  ESPUI.addControl(ControlType::Switcher, "LED enabled", "0",
      ControlColor::Emerald, mainTab, powerCallback);
  auto levelId = ESPUI.addControl(ControlType::Slider, "Brightness, %",
      String(level), ControlColor::Peterriver, mainTab, levelCallback);
  ESPUI.addControl(ControlType::Min, "", "0", ControlColor::None, levelId);
  ESPUI.addControl(ControlType::Max, "", "100", ControlColor::None, levelId);
  ESPUI.begin("ESP32-S3 control panel");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Command cmd;
  while (xQueueReceive(commands, &cmd, 0) == pdTRUE) {
    if (cmd.kind == Kind::Power) on = cmd.value != 0;
    if (cmd.kind == Kind::Level) level = cmd.value;
  }
  ledcWrite(4, on ? level * 255 / 100 : 0);
  delay(10);
}
