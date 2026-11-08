// USB Mode: USB-OTG (TinyUSB); USB CDC On Boot: Disabled.
#include <USB.h>
#include <USBCDC.h>
USBCDC console;
void setup() {
  Serial.begin(115200); // bridge UART for diagnostics with CDC on boot disabled
  console.begin();
  USB.begin();
}
void loop() {
  static char line[32]; static size_t used = 0;
  while (console.available()) {
    char c = console.read();
    if (c == '\n') {
      line[used] = 0;
      if (strcmp(line, "ping") == 0) console.println("pong");
      else console.println("unknown command");
      used = 0;
    } else if (c != '\r') {
      if (used < sizeof(line)-1) line[used++] = c;
      else { used = 0; console.println("line too long"); }
    }
  }
  delay(1);
}
