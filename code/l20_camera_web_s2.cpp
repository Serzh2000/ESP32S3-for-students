void handleCapture() {                    // один JPEG-кадр по адресу /capture
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) { server.send(500, "text/plain", "Кадр не получен"); return; }
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);               // вернуть буфер, иначе кадры закончатся
}

// в setup() после server.on("/", handleRoot):
//   server.on("/capture", handleCapture);
