#include <Arduino.h>
#include <DHT.h>

const int DHT_PIN = 15;                   // DATA датчика + подтяжка 10 кОм к 3V3
const uint32_t PERIOD_MS = 2500;          // DHT22 нельзя опрашивать чаще раза в 2 с

DHT dht(DHT_PIN, DHT22);
uint32_t lastRead = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  if (millis() - lastRead >= PERIOD_MS) {
    lastRead = millis();
    float h = dht.readHumidity();         // влажность, %
    float t = dht.readTemperature();      // температура, °C
    if (isnan(h) || isnan(t)) {           // NaN -- чтение не удалось
      Serial.println("DHT22: ошибка чтения");
    } else {
      Serial.printf("T = %.1f °C, влажность = %.1f %%\n", t, h);
    }
  }
}
