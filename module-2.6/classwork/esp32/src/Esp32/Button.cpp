#include "Esp32/Button.hpp"

Button::Button(std::uint8_t pin, std::uint32_t debounceMs)
    : pin_(pin),
      debounceMs_(debounceMs)
{
}

bool Button::init()
{
    pinMode(pin_, INPUT_PULLUP);

    if (!timer_.init(timerCallback, this)) {
        return false;
    }

    attachInterruptArg(
        digitalPinToInterrupt(pin_), gpioInterrupt, this, CHANGE);
    return true;
}

bool Button::takePress()
{
    portENTER_CRITICAL(&mux_);
    const bool pressed = pressPending_;
    pressPending_ = false;
    portEXIT_CRITICAL(&mux_);
    return pressed;
}

void ARDUINO_ISR_ATTR Button::gpioInterrupt(void* context)
{
    static_cast<Button*>(context)->onGpioInterrupt();
}

void ARDUINO_ISR_ATTR Button::timerCallback(void* context)
{
    static_cast<Button*>(context)->onDebounceTimeout();
}

void ARDUINO_ISR_ATTR Button::onGpioInterrupt()
{
    timer_.start(debounceMs_);
}

void ARDUINO_ISR_ATTR Button::onDebounceTimeout()
{
    const bool pressed = digitalRead(pin_) == LOW;

    if (pressed && !stablePressed_) {
        portENTER_CRITICAL_ISR(&mux_);
        pressPending_ = true;
        portEXIT_CRITICAL_ISR(&mux_);
    }

    stablePressed_ = pressed;
}
