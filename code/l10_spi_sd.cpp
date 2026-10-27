#include <SPI.h>
#include <SD.h>

constexpr int PIN_SCK  = 12;
constexpr int PIN_MISO = 13;
constexpr int PIN_MOSI = 11;
constexpr int PIN_CS   = 10;  // отдельный CS только для SD-карты

SPIClass spi(FSPI);

void setup() {
  Serial.begin(115200);
  spi.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

  // Начинаем с 4 МГц: это надёжнее для макетной платы и длинных проводов.
  if (!SD.begin(PIN_CS, spi, 4000000)) {
    Serial.println("SD-карта не найдена: проверьте питание, CS и общую землю");
    return;
  }

  File log = SD.open("/log.txt", FILE_APPEND);
  if (log) {
    log.println("ESP32-S3 запустился");
    log.close();                 // обязательно: данные уйдут из буфера в карту
    Serial.println("Строка записана в /log.txt");
  }
}

void loop() {}
