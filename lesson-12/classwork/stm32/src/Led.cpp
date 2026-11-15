#include "Led.hpp"

Led::Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeLow)
    : port_(port), pin_(pin), activeLow_(activeLow)
{
}

void Led::off()
{
    write(false);
}

void Led::on()
{
    write(true);
}

void Led::toggle()
{
    write(!enabled_);
}

void Led::write(bool enabled)
{
    enabled_ = enabled;
    const bool pinHigh = activeLow_ ? !enabled_ : enabled_;
    HAL_GPIO_WritePin(port_, pin_, pinHigh ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
