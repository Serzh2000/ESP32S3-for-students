// Deterministic logic bug for a debugger: expected 2.5, printed 2.0.
#include <Arduino.h>
volatile float observed = 0;
float meanWrong(int total, int count) {
  return total / count; // set a breakpoint here; inspect operand types
}
void setup() { Serial.begin(115200); }
void loop() {
  observed = meanWrong(5, 2);
  Serial.printf("mean=%.2f\n", (double)observed);
  delay(1000);
}
