#include "esp_camera.h"
#include "SD_MMC.h"

// Слот microSD (ESP32-S3-EYE): CLK 39, CMD 38, D0 40. Сверьте со своей платой!
const int SD_CLK = 39;
const int SD_CMD = 38;
const int SD_D0  = 40;
const uint32_t SHOT_PERIOD_MS = 5000;     // один снимок раз в 5 секунд
const int MAX_SHOTS = 10;                 // и не больше 10 штук

int shotNo = 0;
uint32_t lastShot = 0;

bool startCamera() {
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 11;  config.pin_d1 = 9;   config.pin_d2 = 8;   config.pin_d3 = 10;
  config.pin_d4 = 12;  config.pin_d5 = 18;  config.pin_d6 = 17;  config.pin_d7 = 16;
  config.pin_xclk = 15;
  config.pin_pclk = 13;
  config.pin_vsync = 6;
  config.pin_href = 7;
  config.pin_sccb_sda = 4;
  config.pin_sccb_scl = 5;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;     // 800x600: для фото чуть больше, чем для потока
  config.jpeg_quality = 10;
  config.fb_count = 1;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  return esp_camera_init(&config) == ESP_OK;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  if (!psramFound() || !startCamera()) {
    Serial.println("Камера не запустилась");
    return;
  }
  SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0);  // режим с одной линией данных (1-bit)
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("microSD не найдена: проверьте карту (FAT32) и выводы слота");
    return;
  }
  Serial.println("Готово: снимки будут сохраняться в /photo_N.jpg");
}

void loop() {
  if (shotNo >= MAX_SHOTS || millis() - lastShot < SHOT_PERIOD_MS) return;
  lastShot = millis();

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) { Serial.println("Кадр не получен"); return; }

  String path = "/photo_" + String(shotNo) + ".jpg";
  File f = SD_MMC.open(path, FILE_WRITE);
  if (f) {
    f.write(fb->buf, fb->len);            // JPEG уже готов -- пишем как есть
    f.close();
    Serial.printf("Сохранено %s (%u байт)\n", path.c_str(), (unsigned)fb->len);
    shotNo++;
  } else {
    Serial.println("Не удалось открыть файл на карте");
  }
  esp_camera_fb_return(fb);               // кадр возвращаем в любом случае
}
