#include <Arduino.h>
// Train two one-dimensional centroids using your own potentiometer samples.
#include <Arduino.h>
float center[2] = {0, 0};
bool trained[2] = {false, false};
void train(int label) {
  uint32_t sum = 0;
  for (int i = 0; i < 100; ++i) { sum += analogRead(1); delay(5); }
  center[label] = sum / (100.0f * 4095.0f);
  trained[label] = true;
  Serial.printf("class %d center=%.3f\n", label, center[label]);
}
void setup() {
  Serial.begin(115200);
  Serial.println("Place knob at class 0, send 0; then at class 1, send 1");
}
void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == '0' || command == '1') train(command - '0');
  }
  if (!trained[0] || !trained[1]) { delay(20); return; }
  float x = analogRead(1) / 4095.0f;
  float d0 = fabsf(x - center[0]), d1 = fabsf(x - center[1]);
  // Reject samples far from both learned classes; this is not a probability.
  int label = min(d0, d1) > 0.12f ? -1 : (d0 < d1 ? 0 : 1);
  Serial.printf("x:%.3f class:%d\n", x, label);
  delay(200);
}
