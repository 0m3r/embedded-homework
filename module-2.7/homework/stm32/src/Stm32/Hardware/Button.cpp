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
    edgePending_ = false;
}

void Button::update()
{
    if (takeEdge()) {
        onEdge(isPressed());
    }

    if (!timer_.takeElapsed()) {
        return;
    }

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

void Button::onInterrupt(std::uint16_t pin)
{
    if (pin != pin_) {
        return;
    }

    edgePending_ = true;
}

void Button::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    timer_.onInterrupt(timer);
}

bool Button::isPressed() const
{
    const bool high = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    return activeHigh_ ? high : !high;
}

bool Button::takeEdge()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const bool edgePending = edgePending_;
    edgePending_ = false;

    if (interruptState == 0U) {
        __enable_irq();
    }

    return edgePending;
}

void Button::onEdge(bool pressed)
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
