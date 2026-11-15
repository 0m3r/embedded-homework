#include "Hardware/Button.hpp"

Button::Button(GPIO_TypeDef* port, std::uint16_t pin, bool activeHigh)
    : port_(port), pin_(pin), activeHigh_(activeHigh)
{
}

void Button::init()
{
    pendingInterrupts_ = 0;
}

void Button::onInterrupt(std::uint16_t pin)
{
    if (pin != pin_) {
        return;
    }

    ++pendingInterrupts_;
}

bool Button::isPressed() const
{
    const bool high = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    return activeHigh_ ? high : !high;
}

std::uint32_t Button::takeInterrupts()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const auto interrupts = pendingInterrupts_;
    pendingInterrupts_ = 0;

    if (interruptState == 0U) {
        __enable_irq();
    }

    return interrupts;
}
