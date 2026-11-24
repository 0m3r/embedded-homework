#include "Debouncer/TimerFsmDebouncer.hpp"

TimerFsmDebouncer::TimerFsmDebouncer(
    const Button& button,
    OneShotTimer& timer,
    std::uint32_t debounceMs)
    : button_(button),
      timer_(timer),
      debounceMs_(debounceMs)
{
}

void TimerFsmDebouncer::init(
    PressCallback callback,
    void* context)
{
    callback_ = callback;
    callbackContext_ = context;
}

void TimerFsmDebouncer::onEdge(bool pressed)
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

void TimerFsmDebouncer::onTimerInterrupt(TIM_HandleTypeDef* timer)
{
    timer_.onInterrupt(timer);
}

void TimerFsmDebouncer::update()
{
    if (!timer_.takeElapsed()) {
        return;
    }

    const bool pressed = button_.isPressed();
    if (state_ == State::Pressed) {
        state_ = pressed ? State::Held : State::Idle;
        if (pressed && callback_ != nullptr) {
            callback_(callbackContext_);
        }
    } else if (state_ == State::Release) {
        state_ = pressed ? State::Held : State::Idle;
    }
}
