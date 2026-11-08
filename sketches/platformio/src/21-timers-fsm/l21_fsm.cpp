#include <Arduino.h>
const int RED_PIN = 4;
const int YELLOW_PIN = 6;
const int GREEN_PIN = 7;
const int BTN_PIN = 5;                        // кнопка пешехода

enum class State { GREEN, YELLOW, RED, RED_YELLOW };

State state = State::GREEN;
uint32_t enteredAt = 0;                       // когда вошли в текущее состояние
bool request = false;                         // пешеход нажал кнопку

void enter(State s) {                         // единственное место смены состояния
  state = s;
  enteredAt = millis();
  digitalWrite(RED_PIN,    s == State::RED || s == State::RED_YELLOW);
  digitalWrite(YELLOW_PIN, s == State::YELLOW || s == State::RED_YELLOW);
  digitalWrite(GREEN_PIN,  s == State::GREEN);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  enter(State::GREEN);
}

void loop() {
  if (digitalRead(BTN_PIN) == LOW) request = true;   // событие запоминаем
  uint32_t t = millis() - enteredAt;                 // время в текущем состоянии

  switch (state) {
    case State::GREEN:                               // машины едут, пока нет запроса
      if (request && t > 5000) enter(State::YELLOW);
      break;
    case State::YELLOW:
      if (t > 2000) enter(State::RED);
      break;
    case State::RED:                                 // пешеходы идут
      if (t > 6000) { request = false; enter(State::RED_YELLOW); }
      break;
    case State::RED_YELLOW:
      if (t > 1500) enter(State::GREEN);
      break;
  }
}
