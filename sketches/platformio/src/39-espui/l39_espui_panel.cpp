#include <Arduino.h>
// LED: GPIO4 -> 220 ohm -> LED -> GND. Potentiometer wiper: GPIO1.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

enum class Kind : uint8_t { Power, Level, Stop };
struct Command { Kind kind; int value; };
QueueHandle_t commands;
std::atomic<unsigned> dropped{0};
uint16_t powerId, levelId, voltageId, stateId, queueId;
bool on = false;
int level = 40;

void enqueue(Kind kind, int value) {
  Command cmd{kind, value};
  if (xQueueSend(commands, &cmd, 0) != pdTRUE) ++dropped;
}
int parsePercent(const String& text) {
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
void stopCallback(Control*, int event) {
  if (event == B_UP) enqueue(Kind::Stop, 0); // once per completed click
}

void setup() {
  Serial.begin(115200);
  commands = xQueueCreate(12, sizeof(Command));
  if (!commands || !ledcAttach(4, 5000, 8)) {
    Serial.println("Queue or PWM init failed");
    while (true) delay(1000);
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-39-demo");
  ESPUI.sliderContinuous = false;
  auto mainTab = ESPUI.addControl(ControlType::Tab, "", "Control");
  auto dataTab = ESPUI.addControl(ControlType::Tab, "", "Measurements");
  powerId = ESPUI.addControl(ControlType::Switcher, "LED enabled", "0",
      ControlColor::Emerald, mainTab, powerCallback);
  levelId = ESPUI.addControl(ControlType::Slider, "Brightness, %",
      String(level), ControlColor::Peterriver, mainTab, levelCallback);
  ESPUI.addControl(ControlType::Min, "", "0", ControlColor::None, levelId);
  ESPUI.addControl(ControlType::Max, "", "100", ControlColor::None, levelId);
  ESPUI.addControl(ControlType::Button, "Output", "All off",
      ControlColor::Alizarin, mainTab, stopCallback);
  stateId = ESPUI.addControl(ControlType::Label, "Applied state", "OFF",
      ControlColor::Turquoise, mainTab);
  voltageId = ESPUI.addControl(ControlType::Label, "ADC1, mV", "0",
      ControlColor::Peterriver, dataTab);
  queueId = ESPUI.addControl(ControlType::Label, "Dropped commands", "0",
      ControlColor::Carrot, dataTab);
  ESPUI.begin("ESP32-S3 control panel");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Command cmd;
  // All changes to our application model and GPIO happen here.
  while (xQueueReceive(commands, &cmd, 0) == pdTRUE) {
    if (cmd.kind == Kind::Power) on = cmd.value != 0;
    if (cmd.kind == Kind::Level) level = cmd.value;
    if (cmd.kind == Kind::Stop) on = false;
  }
  ledcWrite(4, on ? level * 255 / 100 : 0);
  static uint32_t last = 0;
  if (millis() - last >= 500) {
    last = millis();
    ESPUI.updateSwitcher(powerId, on);
    ESPUI.updateSlider(levelId, level);
    ESPUI.updateLabel(stateId, on ? "ON" : "OFF");
    ESPUI.updateLabel(voltageId, String(analogReadMilliVolts(1)));
    ESPUI.updateLabel(queueId, String(dropped.load()));
  }
  delay(1);
}
