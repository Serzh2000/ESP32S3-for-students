// Умная лампа на ESP32-S3 -- итоговый скетч курса
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/rtc_io.h"

// ---------- Выводы ----------
const int LAMP_PIN = 4;                      // светодиод лампы       (урок 1)
const int RGB_PIN  = 48;                     // встроенный RGB, v1.1: 38
const int BTN_PIN  = 5;                      // кнопка на GND         (урок 3)
const int POT_PIN  = 1;                      // потенциометр, ADC1    (урок 5)
const int SDA_PIN  = 8, SCL_PIN = 9;         // OLED SSD1306          (урок 6)

// ---------- Настройки ----------
const char* SSID = "MyNetwork";              // (урок 8)
const char* PASS = "MyPassword";
const uint32_t DEBOUNCE_MS    = 50;          // (урок 3)
const uint32_t SLEEP_AFTER_MS = 60000;       // (урок 9)

// ---------- Состояние (уроки 4, 7, 9) ----------
struct LampState {
  bool on;
  int  level;                                // яркость 0..100 %
};
RTC_DATA_ATTR LampState lamp = {false, 50};  // переживает deep-sleep
SemaphoreHandle_t lampMutex;
TaskHandle_t displayHandle;
volatile uint32_t lastActivity = 0;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
WebServer server(80);

// ---------- Лампа (уроки 4, 7) ----------
uint32_t levelToDuty(int level) {            // гамма-коррекция
  return (uint32_t)(255 * pow(level / 100.0, 2.2) + 0.5);
}

void applyLamp() {                           // вызывать под мьютексом
  ledcWrite(LAMP_PIN, lamp.on ? levelToDuty(lamp.level) : 0);
  lastActivity = millis();
}

LampState getLamp() {
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  LampState copy = lamp;
  xSemaphoreGive(lampMutex);
  return copy;
}

void lampSetOn(bool on) {
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  lamp.on = on;
  applyLamp();
  xSemaphoreGive(lampMutex);
}

void lampToggle() {
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  lamp.on = !lamp.on;
  applyLamp();
  xSemaphoreGive(lampMutex);
}

void lampSetLevel(int level) {
  xSemaphoreTake(lampMutex, portMAX_DELAY);
  lamp.level = constrain(level, 0, 100);
  applyLamp();
  xSemaphoreGive(lampMutex);
}

// ---------- Ввод (уроки 3, 5) ----------
bool buttonPressed() {                       // true один раз на нажатие
  static bool stable = digitalRead(BTN_PIN);
  static bool last = stable;
  static uint32_t changed = 0;
  bool r = digitalRead(BTN_PIN);
  if (r != last) { last = r; changed = millis(); }
  if (millis() - changed > DEBOUNCE_MS && r != stable) {
    stable = r;
    return stable == LOW;
  }
  return false;
}

int readPotLevel() {                         // 0..100 %
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) sum += analogReadMilliVolts(POT_PIN);
  return constrain(map(sum / 8, 0, 3100, 0, 100), 0, 100);
}

// ---------- Экран (урок 6) ----------
void drawDisplay(const LampState& s) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(WiFi.isConnected() ? WiFi.localIP().toString() : String("Smart lamp"));
  display.setTextSize(3);
  display.setCursor(0, 18);
  if (s.on) display.printf("%3d%%", s.level);
  else      display.print("OFF");
  display.drawRect(0, 54, 128, 10, SSD1306_WHITE);
  if (s.on) display.fillRect(2, 56, s.level * 124 / 100, 6, SSD1306_WHITE);
  display.display();
}

// ---------- Задачи (урок 7) ----------
void inputTask(void*) {
  int lastPot = -100;
  for (;;) {
    if (buttonPressed()) lampToggle();
    int pot = readPotLevel();
    if (abs(pot - lastPot) >= 3) { lastPot = pot; lampSetLevel(pot); }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void displayTask(void*) {
  for (;;) {
    drawDisplay(getLamp());
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// ---------- Serial и «пульс» (уроки 1, 2) ----------
void handleSerial() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  if (cmd == "on")                   { lampSetOn(true);  Serial.println("OK"); }
  else if (cmd == "off")             { lampSetOn(false); Serial.println("OK"); }
  else if (cmd.startsWith("level ")) { lampSetLevel(cmd.substring(6).toInt()); Serial.println("OK"); }
  else if (cmd == "status") {
    LampState s = getLamp();
    Serial.printf("lamp=%s level=%d%% ip=%s\n", s.on ? "on" : "off", s.level,
                  WiFi.localIP().toString().c_str());
  }
  else Serial.println("Команды: on, off, level N, status");
}

void heartbeat() {
  static uint32_t last = 0;
  static bool beat = false;
  if (millis() - last >= 500) {
    last = millis();
    beat = !beat;
    rgbLedWrite(RGB_PIN, 0, beat ? 20 : 0, 0);
  }
}

// ---------- Веб (урок 8) ----------
String page() {
  LampState s = getLamp();
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                "<meta name='viewport' content='width=device-width'>"
                "<title>Лампа</title></head><body style='font-family:sans-serif'>";
  html += "<h1>Лампа: ";
  html += s.on ? "ВКЛ" : "ВЫКЛ";
  html += "</h1><p><a href='/toggle'><button>Вкл/выкл</button></a></p>"
          "<form action='/level'>Яркость: <input type='range' name='v' "
          "min='0' max='100' value='" + String(s.level) + "' "
          "onchange='this.form.submit()'></form></body></html>";
  return html;
}

void redirectHome() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void setupWeb() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  server.on("/",       []() { server.send(200, "text/html; charset=utf-8", page()); });
  server.on("/toggle", []() { lampToggle(); redirectHome(); });
  server.on("/level",  []() { lampSetLevel(server.arg("v").toInt()); redirectHome(); });
  server.begin();
}

// ---------- Сон (урок 9) ----------
void goToSleep() {
  vTaskSuspend(displayHandle);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  rgbLedWrite(RGB_PIN, 0, 0, 0);
  rtc_gpio_pullup_en((gpio_num_t)BTN_PIN);
  rtc_gpio_pulldown_dis((gpio_num_t)BTN_PIN);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_PIN, 0);
  Serial.println("Сплю. Разбудит кнопка.");
  Serial.flush();
  esp_deep_sleep_start();
}

// ---------- Точка входа ----------
void setup() {
  Serial.begin(115200);
  rtc_gpio_deinit((gpio_num_t)BTN_PIN);
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) lamp.on = true;

  lampMutex = xSemaphoreCreateMutex();
  ledcAttach(LAMP_PIN, 5000, 8);
  pinMode(BTN_PIN, INPUT_PULLUP);
  applyLamp();

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("Дисплей не найден");
  display.setTextColor(SSD1306_WHITE);

  xTaskCreatePinnedToCore(inputTask,   "input",   4096, NULL, 2, NULL,           1);
  xTaskCreatePinnedToCore(displayTask, "display", 4096, NULL, 1, &displayHandle, 1);
  setupWeb();
}

void loop() {
  handleSerial();
  server.handleClient();
  heartbeat();
  if (!getLamp().on && millis() - lastActivity > SLEEP_AFTER_MS) goToSleep();
  delay(2);
}
