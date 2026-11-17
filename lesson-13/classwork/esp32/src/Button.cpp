#include "Button.hpp"

Button::Button(std::uint8_t pin)
    : pin_(pin)
{
}

void Button::begin()
{
    pinMode(pin_, INPUT_PULLUP);
    attachInterruptArg(
        digitalPinToInterrupt(pin_), interruptHandler, this, FALLING);
}

bool Button::takePress()
{
    noInterrupts();
    const bool pressed = pendingPress_;
    pendingPress_ = false;
    interrupts();
    return pressed;
}

void ARDUINO_ISR_ATTR Button::interruptHandler(void* context)
{
    static_cast<Button*>(context)->onInterrupt();
}

void ARDUINO_ISR_ATTR Button::onInterrupt()
{
    pendingPress_ = true;
}
