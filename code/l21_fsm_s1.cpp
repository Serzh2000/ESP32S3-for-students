const int RED_PIN = 4;
const int YELLOW_PIN = 6;
const int GREEN_PIN = 7;

enum class State { GREEN, YELLOW, RED };      // пока три состояния

State state = State::GREEN;
uint32_t enteredAt = 0;                       // когда вошли в текущее состояние

void enter(State s) {                         // единственное место смены состояния
  state = s;
  enteredAt = millis();
  digitalWrite(RED_PIN,    s == State::RED);
  digitalWrite(YELLOW_PIN, s == State::YELLOW);
  digitalWrite(GREEN_PIN,  s == State::GREEN);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  enter(State::GREEN);
}

void loop() {
  uint32_t t = millis() - enteredAt;          // время в текущем состоянии

  switch (state) {
    case State::GREEN:
      if (t > 5000) enter(State::YELLOW);
      break;
    case State::YELLOW:
      if (t > 2000) enter(State::RED);
      break;
    case State::RED:
      if (t > 6000) enter(State::GREEN);
      break;
  }
}
