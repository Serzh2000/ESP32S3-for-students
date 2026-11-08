#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define LEVEL_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int LED_PIN = 4;

class LevelCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    String v = c->getValue();                   // текст от телефона, например "75"
    int level = constrain(v.toInt(), 0, 100);   // проверяем диапазон до применения
    ledcWrite(LED_PIN, level * 255 / 100);
    Serial.printf("Яркость: %d%%\n", level);
  }
};

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);

  BLEDevice::init("ESP32-S3-Lab");              // имя, которое увидит телефон
  BLEServer* server = BLEDevice::createServer();
  BLEService* service = server->createService(SERVICE_UUID);
  BLECharacteristic* level = service->createCharacteristic(
      LEVEL_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  level->setCallbacks(new LevelCallbacks());
  level->setValue("0");
  service->start();

  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();                // плата видна в списке устройств
}

void loop() { delay(1000); }
