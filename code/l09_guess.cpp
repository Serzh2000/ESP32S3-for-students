// Игра «Угадай число»: плата загадывает число от 1 до 100
int secret;                                  // загаданное число
int tries;                                   // сколько было попыток

void newGame() {
  secret = random(1, 101);                   // случайное от 1 до 100
  tries = 0;
  Serial.println("Я загадала число от 1 до 100. Угадай!");
}

int readNumber() {                           // ждём, пока человек введёт число
  while (Serial.available() == 0) {
    delay(10);
  }
  int n = Serial.parseInt();                 // прочитать число из строки
  while (Serial.available() > 0) Serial.read();   // выбросить остаток строки
  return n;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  randomSeed(esp_random());                  // чтобы числа каждый раз были разными
  newGame();
}

void loop() {
  int guess = readNumber();
  tries++;
  if (guess < secret) {
    Serial.printf("%d -- мало!\n", guess);
  } else if (guess > secret) {
    Serial.printf("%d -- много!\n", guess);
  } else {
    Serial.printf("Угадал за %d попыток! Сыграем ещё.\n", tries);
    newGame();
  }
}
