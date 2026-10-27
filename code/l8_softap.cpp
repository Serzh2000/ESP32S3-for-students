WiFi.softAP("ESP32-S3-Lab", "12345678");    // пароль не короче 8 символов
Serial.println(WiFi.softAPIP());           // обычно 192.168.4.1
