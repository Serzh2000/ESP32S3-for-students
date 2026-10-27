// GPIO4 -> logic analyser, or GPIO5 of the PCNT experiment.
#include <Arduino.h>
rmt_data_t pulse[2];
bool ready = false;
void setup() {
  Serial.begin(115200);
  ready = rmtInit(4, RMT_TX_MODE, RMT_MEM_NUM_BLOCKS_1, 1000000);
  pulse[0].duration0 = 100; pulse[0].level0 = 1;
  pulse[0].duration1 = 200; pulse[0].level1 = 0;
  pulse[1].duration0 = 300; pulse[1].level0 = 1;
  pulse[1].duration1 = 400; pulse[1].level1 = 0;
  if (!ready) Serial.println("RMT init failed");
}
void loop() {
  if (ready && !rmtWrite(4, pulse, 2, 100))
    Serial.println("RMT timeout");
  delay(1000);
}
