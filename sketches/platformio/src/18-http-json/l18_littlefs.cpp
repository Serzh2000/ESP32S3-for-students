#include <Arduino.h>
#include <LittleFS.h>

const char* PATH = "/settings.txt";

void setup() {
  Serial.begin(115200);
  if (!LittleFS.begin(true)) {            // true: отформатировать раздел при первом запуске
    Serial.println("LittleFS не запустилась");
    return;
  }

  File f = LittleFS.open(PATH, "r");      // читаем, если файл уже есть
  if (f) {
    int level = f.readStringUntil('\n').toInt();
    f.close();
    Serial.printf("Прочитано: уровень = %d\n", level);
  } else {
    Serial.println("Файла настроек ещё нет");
  }

  File w = LittleFS.open(PATH, "w");      // "w" стирает старое содержимое
  if (w) {
    w.println(75);                        // новое значение настройки
    w.close();                            // закрыть, чтобы данные записались
    Serial.println("Настройка записана");
  }
}

void loop() {}
