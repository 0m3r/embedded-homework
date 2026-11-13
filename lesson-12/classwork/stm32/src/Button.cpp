#include "Button.hpp"

Button::Button(const Clock& clock, std::uint16_t pin, std::uint32_t debounceMs)
    : pin_(pin), debounceTimer_(clock, debounceMs)
{
}

void Button::init()
{
    debounceTimer_.reset();
    pressed_ = false;
}

void Button::onInterrupt(std::uint16_t pin)
{
    if (pin != pin_) {
        return;
    }

    if (!debounceElapsed()) {
        return;
    }

    debounceTimer_.reset();
    pressed_ = true;
}

bool Button::debounceElapsed() const
{
    return debounceTimer_.elapsed();
}

bool Button::wasPressed()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const bool pressed = pressed_;
    pressed_ = false;

    if (interruptState == 0U) {
        __enable_irq();
    }

    return pressed;
}
