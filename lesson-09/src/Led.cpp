#include "Led.h"

#include <Arduino.h>

void Led::init()
{
    pinMode(pin_, OUTPUT);
    set(LedState::Off);
}

void Led::set(LedState state)
{
    state_ = state;
    digitalWrite(pin_, state_ == LedState::On ? HIGH : LOW);
}

LedState Led::state() const
{
    return state_;
}
