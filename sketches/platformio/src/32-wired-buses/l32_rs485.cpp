#include <Arduino.h>
// 3.3V RS-485 module: DI=16, RO=17, DE and /RE joined on GPIO4.
#include <Arduino.h>
const bool MASTER = true; // second board: false
const int DIR = 4;
void sendFrame(const char* frame) {
  digitalWrite(DIR, HIGH);
  delayMicroseconds(10);
  Serial1.print(frame);
  Serial1.flush(); // wait for the final stop bit before releasing the bus
  digitalWrite(DIR, LOW);
}
void setup() {
  Serial.begin(115200);
  pinMode(DIR, OUTPUT); digitalWrite(DIR, LOW);
  Serial1.begin(9600, SERIAL_8N1, 17, 16);
}
void loop() {
  static uint32_t last = 0;
  if (MASTER && millis() - last >= 1000) {
    last = millis(); sendFrame("1,PING\n");
  }
  static char line[48]; static size_t used = 0;
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      line[used] = 0; Serial.println(line);
      if (!MASTER && strcmp(line, "1,PING") == 0) {
        delay(2); sendFrame("0,PONG\n");
      }
      used = 0;
    } else if (used < sizeof(line) - 1) line[used++] = c;
    else used = 0;
  }
}
