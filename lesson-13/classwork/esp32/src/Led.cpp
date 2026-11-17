#include "Led.hpp"

#include <Arduino.h>

Led::Led(std::uint8_t pin)
    : pin_(pin)
{
}

void Led::begin()
{
    pinMode(pin_, OUTPUT);
    off();
}

void Led::off()
{
    write(false);
}

void Led::toggle()
{
    write(!enabled_);
}

void Led::write(bool enabled)
{
    enabled_ = enabled;
    digitalWrite(pin_, enabled_ ? HIGH : LOW);
}
