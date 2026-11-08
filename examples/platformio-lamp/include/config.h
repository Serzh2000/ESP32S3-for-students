#pragma once
#include <stdint.h>

// Все «магические» числа проекта собраны здесь (урок 28).
constexpr int BUTTON_PIN = 5;            // кнопка между GPIO5 и GND, внутренняя подтяжка
constexpr int LED_PIN = 4;               // светодиод через резистор 220 Ом
constexpr uint32_t DEBOUNCE_MS = 50;     // сколько мс уровень должен быть неизменным
