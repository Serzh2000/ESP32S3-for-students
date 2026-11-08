#include <Arduino.h>
// Шаг 2 (урок 26): угол наклона только по акселерометру.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const uint8_t REG_PWR_MGMT_1 = 0x6B;      // управление питанием
const uint8_t REG_ACCEL_XOUT = 0x3B;      // отсюда подряд идут 14 байт измерений
const uint8_t REG_WHO_AM_I = 0x75;        // «паспорт» микросхемы

void writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

bool readRegs(uint8_t reg, uint8_t* buf, size_t n) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);                                    // с какого регистра читать
  if (Wire.endTransmission(false) != 0) return false; // датчик не ответил
  if (Wire.requestFrom(MPU_ADDR, n) != n) return false;
  for (size_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  uint8_t id = 0;
  if (!readRegs(REG_WHO_AM_I, &id, 1)) {
    Serial.println("MPU6050 не отвечает: проверьте провода и адрес");
  } else {
    Serial.printf("WHO_AM_I = 0x%02X\n", id);
  }
  writeReg(REG_PWR_MGMT_1, 0);            // после включения датчик спит -- будим
}

void loop() {
  uint8_t b[14];
  if (!readRegs(REG_ACCEL_XOUT, b, 14)) { delay(500); return; }

  int16_t ay = (int16_t)((b[2] << 8) | b[3]);         // два байта -> число со знаком
  int16_t az = (int16_t)((b[4] << 8) | b[5]);

  float accAngle = atan2(ay, az) * 57.2958;           // наклон по акселерометру
  Serial.printf("acc:%.1f\n", accAngle);              // формат для Serial Plotter
  delay(20);
}
