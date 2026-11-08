#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_PIN = 7;               // DATA датчика + подтяжка 4,7 кОм к 3V3

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  sensors.setResolution(12);              // 12 бит: шаг 0,0625 °C, ~750 мс на замер
  Serial.printf("Найдено датчиков: %d\n", sensors.getDeviceCount());
}

void loop() {
  sensors.requestTemperatures();          // запустить замер (ждёт окончания)
  float t = sensors.getTempCByIndex(0);   // первый датчик на шине
  if (t == DEVICE_DISCONNECTED_C) {       // -127: датчик не отвечает
    Serial.println("DS18B20 не отвечает: проверьте подтяжку и провода");
  } else {
    Serial.printf("Температура: %.2f °C\n", t);
  }
  delay(2000);
}
