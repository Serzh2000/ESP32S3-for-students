// Какие Bluetooth-устройства рядом?
#include <BLEDevice.h>

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");                       // включить Bluetooth
}

void loop() {
  Serial.println("Слушаю эфир 5 секунд...");
  BLEScan* scan = BLEDevice::getScan();
  scan->setActiveScan(true);                 // просить устройства назвать себя
  BLEScanResults* found = scan->start(5, false);
  for (int i = 0; i < found->getCount(); i++) {
    BLEAdvertisedDevice d = found->getDevice(i);
    Serial.printf("%s  сигнал %4d дБм  %s\n", d.getAddress().toString().c_str(),
                  d.getRSSI(), d.getName().c_str());
  }
  scan->clearResults();
  delay(5000);
}
