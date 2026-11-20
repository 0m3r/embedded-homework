#include "Stm32/Button.hpp"

Button::Button(GPIO_TypeDef* port,
               std::uint16_t pin,
               bool pressedHigh,
               std::uint32_t pull)
    : Button(port, pin, pressedHigh, pull, nullptr, 0)
{
}

Button::Button(GPIO_TypeDef* port,
               std::uint16_t pin,
               bool pressedHigh,
               std::uint32_t pull,
               TIM_TypeDef* timer,
               std::uint32_t debounceMs)
    : port_(port),
      pin_(pin),
      pressedHigh_(pressedHigh),
      pull_(pull),
      debounceMs_(debounceMs),
      interrupt_(pin == GPIO_PIN_0 ? EXTI0_IRQn : EXTI9_5_IRQn),
      timer_(timer),
      softwareDebounce_(timer != nullptr)
{
}

bool Button::init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {};
    gpio.Pin = pin_;
    gpio.Mode = pressedHigh_ ? GPIO_MODE_IT_RISING : GPIO_MODE_IT_FALLING;
    gpio.Pull = pull_;
    HAL_GPIO_Init(port_, &gpio);

    const std::uint32_t subPriority = pin_ == GPIO_PIN_0 ? 1U : 2U;
    HAL_NVIC_SetPriority(interrupt_, 2, subPriority);
    HAL_NVIC_EnableIRQ(interrupt_);

    return !softwareDebounce_ || timer_.init(timerCallback, this);
}

void Button::onInterrupt(std::uint16_t pin)
{
    if (pin != pin_) {
        return;
    }

    if (softwareDebounce_) {
        HAL_NVIC_DisableIRQ(interrupt_);
        timer_.start(debounceMs_);
    } else {
        pressPending_ = true;
    }
}

bool Button::takePress()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const bool pressed = pressPending_;
    pressPending_ = false;

    if (interruptState == 0U) {
        __enable_irq();
    }
    return pressed;
}

void Button::timerCallback(void* context)
{
    static_cast<Button*>(context)->onDebounceTimeout();
}

void Button::onDebounceTimeout()
{
    const bool high = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    const bool pressed = high == pressedHigh_;

    if (pressed) {
        pressPending_ = true;
    }

    __HAL_GPIO_EXTI_CLEAR_IT(pin_);
    HAL_NVIC_ClearPendingIRQ(interrupt_);
    HAL_NVIC_EnableIRQ(interrupt_);
}
