#include "Hardware/ButtonInput.hpp"

ButtonInput::ButtonInput(
    GPIO_TypeDef* port,
    std::uint16_t pin,
    bool activeHigh)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh)
{
}

bool ButtonInput::isPressed() const
{
    const bool high = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    return activeHigh_ ? high : !high;
}
