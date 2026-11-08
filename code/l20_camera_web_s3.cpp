const uint32_t MAX_STREAM_MS = 10UL * 60 * 1000;   // поток сам закончится через 10 минут

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

// в setup() после server.on("/capture", handleCapture):
//   server.on("/stream", handleStream);
