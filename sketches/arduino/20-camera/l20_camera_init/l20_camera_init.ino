#include "esp_camera.h"

// Выводы камеры для плат типа ESP32-S3-EYE (CAMERA_MODEL_ESP32S3_EYE).
// Сверьте их со схемой своей платы!
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  15
#define SIOD_GPIO_NUM  4
#define SIOC_GPIO_NUM  5
#define Y9_GPIO_NUM    16   // D7
#define Y8_GPIO_NUM    17   // D6
#define Y7_GPIO_NUM    18   // D5
#define Y6_GPIO_NUM    12   // D4
#define Y5_GPIO_NUM    10   // D3
#define Y4_GPIO_NUM    8    // D2
#define Y3_GPIO_NUM    9    // D1
#define Y2_GPIO_NUM    11   // D0
#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM  7
#define PCLK_GPIO_NUM  13

bool startCamera() {
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;         // тактовая частота датчика, 20 МГц
  config.pixel_format = PIXFORMAT_JPEG;   // сжатие в JPEG делает сам датчик
  config.frame_size = FRAMESIZE_VGA;      // 640x480
  config.jpeg_quality = 12;               // 0..63: меньше число -- лучше качество
  config.fb_count = 2;                    // два буфера кадра в PSRAM
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;  // при отставании брать самый свежий кадр

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Камера не запустилась, код 0x%x\n", err);
    return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);                            // дать USB-порту время подключиться
  if (!psramFound()) {
    Serial.println("PSRAM не найдена: выберите Tools -> PSRAM -> OPI PSRAM");
    return;
  }
  Serial.printf("PSRAM: %u байт\n", (unsigned)ESP.getPsramSize());
  if (!startCamera()) return;

  camera_fb_t *fb = esp_camera_fb_get();  // взять кадр из очереди драйвера
  if (!fb) {
    Serial.println("Кадр не получен");
    return;
  }
  Serial.printf("Кадр: %ux%u, %u байт JPEG\n",
                (unsigned)fb->width, (unsigned)fb->height, (unsigned)fb->len);
  esp_camera_fb_return(fb);               // обязательно вернуть буфер драйверу
}

void loop() {}
