#include <Arduino.h>
// Шаг 2. Очередь команд: callback кладёт команду, loop() её выполняет.
// LED: GPIO4 -> 220 ohm -> LED -> GND.
#include <WiFi.h>
#include <ESPUI.h>
#include <atomic>

enum class Kind : uint8_t { Power };
struct Command { Kind kind; int value; };
QueueHandle_t commands;
std::atomic<unsigned> dropped{0};         // сколько команд не влезло в очередь
bool on = false;                          // состояние принадлежит только loop()

void enqueue(Kind kind, int value) {
  Command cmd{kind, value};
  if (xQueueSend(commands, &cmd, 0) != pdTRUE) ++dropped;   // 0: не ждать
}

void powerCallback(Control*, int event) {
  if (event == S_ACTIVE)   enqueue(Kind::Power, 1);
  if (event == S_INACTIVE) enqueue(Kind::Power, 0);
}

void setup() {
  Serial.begin(115200);
  commands = xQueueCreate(12, sizeof(Command));
  ledcAttach(4, 5000, 8);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("S3-Panel", "lesson-39-demo");
  auto mainTab = ESPUI.addControl(ControlType::Tab, "", "Control");
  ESPUI.addControl(ControlType::Switcher, "LED enabled", "0",
      ControlColor::Emerald, mainTab, powerCallback);
  ESPUI.begin("ESP32-S3 control panel");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Command cmd;
  while (xQueueReceive(commands, &cmd, 0) == pdTRUE)
    if (cmd.kind == Kind::Power) on = cmd.value != 0;
  ledcWrite(4, on ? 255 : 0);
  delay(10);
}
