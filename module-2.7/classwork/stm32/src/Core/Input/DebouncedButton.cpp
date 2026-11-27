#include "Core/Input/DebouncedButton.hpp"

DebouncedButton::DebouncedButton(
    IButtonInput& input,
    IOneShotTimer& timer,
    std::uint32_t debounceMs)
    : input_(input),
      timer_(timer),
      debounceMs_(debounceMs)
{
}

void DebouncedButton::init(PressCallback callback, void* context)
{
    callback_ = callback;
    callbackContext_ = context;
    state_ = State::Idle;
}

void DebouncedButton::onEdge()
{
    handleEdge(input_.isPressed());
}

void DebouncedButton::onTimeout()
{
    const bool pressed = input_.isPressed();
    if (state_ == State::Pressed) {
        state_ = pressed ? State::Held : State::Idle;
        if (pressed && callback_ != nullptr) {
            callback_(callbackContext_);
        }
    } else if (state_ == State::Release) {
        state_ = pressed ? State::Held : State::Idle;
    }
}

void DebouncedButton::handleEdge(bool pressed)
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
