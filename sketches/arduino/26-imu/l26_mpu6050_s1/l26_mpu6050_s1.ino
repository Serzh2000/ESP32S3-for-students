// Шаг 1 (урок 26): проверяем связь с MPU6050. SDA -> GPIO8, SCL -> GPIO9.
#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;
const uint8_t REG_PWR_MGMT_1 = 0x6B;      // управление питанием
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
}
