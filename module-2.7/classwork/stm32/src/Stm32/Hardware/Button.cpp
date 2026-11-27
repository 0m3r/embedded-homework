#include "Hardware/Button.hpp"

Button::Button(
    GPIO_TypeDef* port,
    std::uint16_t pin,
    bool activeHigh,
    TIM_HandleTypeDef& timer,
    std::uint32_t debounceMs)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh),
      debounceMs_(debounceMs),
      timer_(timer)
{
}

void Button::init(PressCallback callback, void* context)
{
    callback_ = callback;
    callbackContext_ = context;
    state_ = State::Idle;
}

void Button::onEdge()
{
    handleEdge(isPressed());
}

void Button::onTimeout()
{
    const bool pressed = isPressed();
    if (state_ == State::Pressed) {
        state_ = pressed ? State::Held : State::Idle;
        if (pressed && callback_ != nullptr) {
            callback_(callbackContext_);
        }
    } else if (state_ == State::Release) {
        state_ = pressed ? State::Held : State::Idle;
    }
}

bool Button::isPressed() const
{
    const bool high = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    return activeHigh_ ? high : !high;
}

void Button::handleEdge(bool pressed)
{
    switch (state_) {
    case State::Idle:
        if (pressed) {
            state_ = State::Pressed;
            timer_.start(debounceMs_);
        }
        break;

    case State::Pressed:
        timer_.start(debounceMs_);
        break;

    case State::Held:
        if (!pressed) {
            state_ = State::Release;
            timer_.start(debounceMs_);
        }
        break;

    case State::Release:
        timer_.start(debounceMs_);
        break;
    }
}
