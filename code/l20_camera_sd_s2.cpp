const uint32_t SHOT_PERIOD_MS = 5000;     // один снимок раз в 5 секунд
const int MAX_SHOTS = 10;                 // и не больше 10 штук

int shotNo = 0;
uint32_t lastShot = 0;

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
