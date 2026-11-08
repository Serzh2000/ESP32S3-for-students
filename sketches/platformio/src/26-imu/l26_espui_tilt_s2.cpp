#include <Arduino.h>
// Шаг 2 (урок 26): настоящий угол по акселерометру. SDA -> GPIO8, SCL -> GPIO9, адрес 0x68.
#include <Wire.h>
#include <WiFi.h>
#include <ESPUI.h>

const uint8_t MPU_ADDR = 0x68;
bool sensorOk = false;
uint16_t graphId, accId, stateId;

void writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR); Wire.write(reg); Wire.write(value); Wire.endTransmission();
}

bool readRegs(uint8_t reg, uint8_t* buf, size_t n) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0 || Wire.requestFrom(MPU_ADDR, n) != n) return false;
  for (size_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  writeReg(0x6B, 0);                      // разбудить датчик
  uint8_t id;
  sensorOk = readRegs(0x75, &id, 1);      // WHO_AM_I: ответ вообще есть?
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Tilt", "lesson-39-demo")) { Serial.println("AP failed"); return; }
  stateId = ESPUI.label("Датчик", ControlColor::Peterriver, sensorOk ? "отвечает" : "НЕ ОТВЕЧАЕТ");
  accId  = ESPUI.label("Угол по акселерометру, °", ControlColor::Alizarin, "0");
  graphId = ESPUI.graph("Угол", ControlColor::Turquoise);
  ESPUI.begin("ESP32-S3 tilt");
}

void loop() {
  static uint32_t lastShow = 0;
  uint8_t b[14];
  if (!readRegs(0x3B, b, 14)) {           // нет ответа: честно говорим об этом
    ESPUI.updateLabel(stateId, "НЕ ОТВЕЧАЕТ"); delay(500); return;
  }
  int16_t ay = (int16_t)((b[2] << 8) | b[3]), az = (int16_t)((b[4] << 8) | b[5]);
  float accAngle = atan2(ay, az) * 57.2958f;
  if (millis() - lastShow >= 250) {
    lastShow = millis();
    ESPUI.updateLabel(stateId, "отвечает");
    ESPUI.updateLabel(accId, String(accAngle, 1));
    ESPUI.addGraphPoint(graphId, (int)accAngle);
  }
  delay(10);
}
