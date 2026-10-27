// INMP441: VDD=3V3, GND=GND, L/R=GND (left slot).
#include <ESP_I2S.h>
#include <math.h>
I2SClass mic;
bool ready = false;
int32_t samples[256];
void setup() {
  Serial.begin(115200);
  mic.setPins(16, 17, -1, 18); // BCLK, WS, DOUT unused, DIN
  ready = mic.begin(I2S_MODE_STD, 16000,
                    I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO,
                    I2S_STD_SLOT_LEFT);
  if (!ready) Serial.println("I2S init failed");
}
void loop() {
  if (!ready) { delay(100); return; }
  size_t bytes = mic.readBytes((char*)samples, sizeof(samples));
  size_t n = bytes / sizeof(samples[0]);
  if (!n) return;
  double mean = 0, energy = 0;
  // INMP441: signed sample is left-aligned in the 32-bit slot.
  for (size_t i = 0; i < n; ++i) mean += samples[i] / 256.0;
  mean /= n;
  for (size_t i = 0; i < n; ++i) {
    double x = samples[i] / 256.0 - mean;
    energy += x * x;
  }
  Serial.printf("rms:%.1f\n", sqrt(energy / n));
  delay(20);
}
