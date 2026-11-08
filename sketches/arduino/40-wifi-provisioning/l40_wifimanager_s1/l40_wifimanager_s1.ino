// Шаг 1. Подключение по сохранённым данным и независимое мигание LED (GPIO4 через резистор).
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT); digitalWrite(4, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin();                              // сеть, сохранённая на плате раньше
  Serial.println("Trying stored Wi-Fi credentials");
}

void loop() {
  uint32_t now = millis();
  static uint32_t last = 0;
  static bool led = false;
  if (now - last >= 500) {                   // мигание не зависит от сети
    last = now; led = !led; digitalWrite(4, led);
  }
  static bool reported = false;
  if (WiFi.status() == WL_CONNECTED && !reported) {
    Serial.print("Connected: "); Serial.println(WiFi.localIP());
    reported = true;
  }
  if (WiFi.status() != WL_CONNECTED) reported = false;
  delay(1);
}
