#include "Button.hpp"

#include <Arduino.h>

Button::Button(std::uint8_t pin)
    : pin_(pin)
{
}

void Button::begin()
{
    // External 10 kOhm pull-up to 3.3 V.
    pinMode(pin_, INPUT);
    attachInterruptArg(digitalPinToInterrupt(pin_), interruptHandler, this, FALLING);
}

bool Button::isPressed() const
{
    return digitalRead(pin_) == LOW;
}

std::uint32_t Button::takeInterrupts()
{
    noInterrupts();
    const auto count = pendingInterrupts_;
    pendingInterrupts_ = 0;
    interrupts();
    return count;
}

void IRAM_ATTR Button::interruptHandler(void* context)
{
    static_cast<Button*>(context)->onInterrupt();
}

void IRAM_ATTR Button::onInterrupt()
{
    pendingInterrupts_ = pendingInterrupts_ + 1U;
}
