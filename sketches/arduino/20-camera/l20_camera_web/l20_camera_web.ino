#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"

const char* SSID = "MyNetwork";
const char* PASS = "MyPassword";
const uint32_t MAX_STREAM_MS = 10UL * 60 * 1000;   // поток сам закончится через 10 минут

WebServer server(80);

bool startCamera() {                      // выводы ESP32-S3-EYE, сверьте со схемой
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 11;  config.pin_d1 = 9;   config.pin_d2 = 8;   config.pin_d3 = 10;
  config.pin_d4 = 12;  config.pin_d5 = 18;  config.pin_d6 = 17;  config.pin_d7 = 16;
  config.pin_xclk = 15;  config.pin_pclk = 13;
  config.pin_vsync = 6;  config.pin_href = 7;
  config.pin_sccb_sda = 4;  config.pin_sccb_scl = 5;
  config.pin_pwdn = -1;  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;      // для потока хватает 640x480
  config.jpeg_quality = 12;
  config.fb_count = 2;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;
  return esp_camera_init(&config) == ESP_OK;
}

void handleRoot() {                       // страница с картинкой, подключённой к потоку
  server.send(200, "text/html",
              "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body>"
              "<h3>ESP32-S3 CAM</h3><img src='/stream' width='640'>"
              "<p><a href='/capture'>Один снимок</a></p></body></html>");
}

void handleCapture() {                    // один JPEG-кадр по адресу /capture
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) { server.send(500, "text/plain", "Кадр не получен"); return; }
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);               // вернуть буфер, иначе кадры закончатся
}

void handleStream() {                     // MJPEG: непрерывная череда JPEG-кадров
  WiFiClient client = server.client();
  client.print("HTTP/1.1 200 OK\r\n"
               "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n");
  uint32_t start = millis();
  while (client.connected() && millis() - start < MAX_STREAM_MS) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) break;
    size_t len = fb->len;
    client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                  (unsigned)len);
    size_t sent = client.write(fb->buf, len);
    client.print("\r\n");
    esp_camera_fb_return(fb);             // вернуть кадр до любого выхода из цикла
    if (sent != len) break;               // клиент отключился -- выходим
  }
}

void setup() {
  Serial.begin(115200);
  if (!psramFound() || !startCamera()) {
    Serial.println("Камера не запустилась: проверьте PSRAM, шлейф и выводы");
    return;
  }
  WiFi.begin(SSID, PASS);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) delay(300);
  if (WiFi.status() != WL_CONNECTED) { Serial.println("Нет Wi-Fi"); return; }
  Serial.print("Откройте в браузере: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.on("/stream", handleStream);
  server.begin();
}

void loop() {
  server.handleClient();
}
