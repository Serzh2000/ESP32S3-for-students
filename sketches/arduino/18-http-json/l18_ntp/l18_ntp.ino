#include <WiFi.h>
#include <time.h>

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }

  // Москва: UTC+3 без перехода на летнее время. В записи POSIX знак обратный: "MSK-3".
  configTzTime("MSK-3", "pool.ntp.org", "time.nist.gov");
}

void loop() {
  struct tm t;
  if (getLocalTime(&t, 5000)) {           // ждать синхронизации не дольше 5 с
    Serial.println(&t, "%d.%m.%Y %H:%M:%S");
  } else {
    Serial.println("Время ещё не получено");
  }
  delay(1000);
}
