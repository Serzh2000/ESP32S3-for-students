#include <Arduino.h>
void setup() {
  Serial.begin(115200);
  delay(1000);                            // дать USB-порту время подключиться
  if (!psramFound()) {
    Serial.println("PSRAM не найдена: выберите Tools -> PSRAM -> OPI PSRAM");
    return;
  }
  Serial.printf("PSRAM: %u байт\n", (unsigned)ESP.getPsramSize());
}

void loop() {}
