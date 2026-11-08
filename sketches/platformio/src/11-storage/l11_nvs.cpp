#include <Arduino.h>
#include <Preferences.h>

Preferences prefs;
uint8_t brightness;

void setup() {
  Serial.begin(115200);
  prefs.begin("app", false);                   // пространство имён, запись разрешена
  brightness = prefs.getUChar("level", 50);    // 50 -- значение по умолчанию
  Serial.printf("Сохранённая яркость: %u%%\n", brightness);
}

void loop() {
  // Вызывайте только когда пользователь закончил менять настройку,
  // а не на каждом проходе loop().
  if (Serial.available()) {
    int newLevel = Serial.parseInt();
    if (newLevel >= 0 && newLevel <= 100) {
      brightness = newLevel;
      prefs.putUChar("level", brightness);
      Serial.println("Яркость сохранена в NVS");
    }
  }
}
