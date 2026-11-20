#include "Stm32/Led.hpp"

Led::Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeHigh)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh)
{
}

void Led::init()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(
        port_, pin_, activeHigh_ ? GPIO_PIN_RESET : GPIO_PIN_SET);

    GPIO_InitTypeDef gpio = {};
    gpio.Pin = pin_;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port_, &gpio);
}

void Led::toggle()
{
    on_ = !on_;
    const bool high = on_ == activeHigh_;
    HAL_GPIO_WritePin(port_, pin_, high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool Led::isOn() const
{
    return on_;
}
