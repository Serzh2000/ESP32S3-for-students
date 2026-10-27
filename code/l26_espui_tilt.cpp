// MPU6050 (GY-521): SDA -> GPIO8, SCL -> GPIO9, адрес 0x68 (урок 26). График наклона на телефоне.
#include <Wire.h>
#include <WiFi.h>
#include <ESPUI.h>

const uint8_t MPU_ADDR = 0x68;
float angle = 0;
uint32_t lastUs = 0;
bool sensorOk = false;
uint16_t graphId, accId, filtId, stateId;

void writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

bool readRegs(uint8_t reg, uint8_t* buf, size_t n) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, n) != n) return false;
  for (size_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  writeReg(0x6B, 0);                      // разбудить датчик
  uint8_t id = 0;
  sensorOk = readRegs(0x75, &id, 1);      // WHO_AM_I: ответ вообще есть?
  lastUs = micros();

  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP("S3-Tilt", "lesson-39-demo")) { Serial.println("AP failed"); return; }
  stateId = ESPUI.label("Датчик", ControlColor::Peterriver, sensorOk ? "отвечает" : "НЕ ОТВЕЧАЕТ");
  accId  = ESPUI.label("Угол по акселерометру, °", ControlColor::Alizarin, "0");
  filtId = ESPUI.label("Угол после фильтра, °", ControlColor::Emerald, "0");
  graphId = ESPUI.graph("Фильтрованный угол", ControlColor::Turquoise);
  ESPUI.begin("ESP32-S3 tilt");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static uint32_t lastShow = 0;
  uint8_t b[14];
  if (!readRegs(0x3B, b, 14)) {           // нет ответа: честно говорим об этом
    ESPUI.updateLabel(stateId, "НЕ ОТВЕЧАЕТ");
    delay(500);
    return;
  }
  int16_t ay = (int16_t)((b[2] << 8) | b[3]);
  int16_t az = (int16_t)((b[4] << 8) | b[5]);
  int16_t gx = (int16_t)((b[8] << 8) | b[9]);

  uint32_t now = micros();
  float dt = (now - lastUs) / 1000000.0f;
  lastUs = now;
  float accAngle = atan2(ay, az) * 57.2958f;
  angle = 0.98f * (angle + (gx / 131.0f) * dt) + 0.02f * accAngle;

  if (millis() - lastShow >= 250) {       // фильтр считаем часто, показываем редко
    lastShow = millis();
    ESPUI.updateLabel(stateId, "отвечает");
    ESPUI.updateLabel(accId, String(accAngle, 1));
    ESPUI.updateLabel(filtId, String(angle, 1));
    ESPUI.addGraphPoint(graphId, (int)angle);
  }
  delay(10);
}
