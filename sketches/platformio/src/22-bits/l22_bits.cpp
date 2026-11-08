#include <Arduino.h>
void printBits(const char* label, uint8_t v) {
  Serial.printf("%-22s", label);
  for (int i = 7; i >= 0; i--) Serial.print((v >> i) & 1);   // от старшего бита к младшему
  Serial.printf("  = 0x%02X = %u\n", v, v);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  uint8_t flags = 0;
  printBits("начало", flags);

  flags |= (1 << 3);                 // установить бит 3
  printBits("установили бит 3", flags);

  flags |= (1 << 0) | (1 << 6);      // установить сразу два бита
  printBits("установили 0 и 6", flags);

  flags &= ~(1 << 3);                // сбросить бит 3
  printBits("сбросили бит 3", flags);

  flags ^= (1 << 7);                 // переключить бит 7
  printBits("переключили бит 7", flags);

  if (flags & (1 << 6)) Serial.println("бит 6 установлен");   // проверить бит

  uint16_t value = 0x1234;
  uint8_t high = value >> 8;         // старший байт: 0x12
  uint8_t low  = value & 0xFF;       // младший байт: 0x34
  Serial.printf("0x%04X -> старший 0x%02X, младший 0x%02X\n", value, high, low);
}

void loop() {}
