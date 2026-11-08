// Переменные: коробки с названиями
int apples = 5;                    // целое число
float temperature = 23.5;          // дробное число (точка вместо запятой!)
bool isOpen = true;                // логическое: true (да) или false (нет)
String name = "Маша";              // строка текста
const int DAYS_IN_WEEK = 7;        // константа: менять нельзя

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.print("Яблок: ");
  Serial.println(apples);          // 5

  apples = apples + 3;             // взять старое значение, прибавить 3, положить обратно
  Serial.print("Стало яблок: ");
  Serial.println(apples);          // 8

  apples++;                        // то же, что apples = apples + 1
  Serial.println(apples);          // 9

  Serial.println(7 / 2);           // 3   -- целые числа делятся нацело!
  Serial.println(7 % 2);           // 1   -- остаток от деления
  Serial.println(7.0 / 2);         // 3.50 -- а дробные -- как обычно

  Serial.println("Привет, " + name + "!");
  Serial.printf("На улице %.1f градусов\n", temperature);
}

void loop() {}
