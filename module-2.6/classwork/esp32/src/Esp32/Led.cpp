#include "Esp32/Led.hpp"

#include <Arduino.h>

Led::Led(std::uint8_t pin)
    : pin_(pin)
{
}

void Led::init()
{
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
}

void Led::toggle()
{
    on_ = !on_;
    digitalWrite(pin_, on_ ? HIGH : LOW);
}

bool Led::isOn() const
{
    return on_;
}
