#include "EspIdf/Debouncer/TimerFsmDebouncer.hpp"

TimerFsmDebouncer::TimerFsmDebouncer(
    const Button& button,
    std::uint32_t debounceMs)
    : button_(button),
      debounceMs_(debounceMs)
{
}

esp_err_t TimerFsmDebouncer::init(
    PressCallback callback,
    void* context)
{
    if (callback == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    callback_ = callback;
    callbackContext_ = context;

    return timer_.init(timerCallback, this);
}

void TimerFsmDebouncer::onEdge(bool pressed)
{
    bool restart = false;
    portENTER_CRITICAL(&mux_);

    switch (state_) {
    case State::Idle:
        if (pressed) {
            state_ = State::Pressed;
            restart = true;
        }
        break;

    case State::Pressed:
        restart = true;
        break;

    case State::Held:
        if (!pressed) {
            state_ = State::Release;
            restart = true;
        }
        break;

    case State::Release:
        restart = true;
        break;
    }

    portEXIT_CRITICAL(&mux_);

    if (restart) {
        timer_.start(debounceMs_);
    }
}

void TimerFsmDebouncer::timerCallback(void* context)
{
    static_cast<TimerFsmDebouncer*>(context)->onTimeout();
}

void TimerFsmDebouncer::onTimeout()
{
    const bool pressed = button_.isPressed();
    bool emitPress = false;
    portENTER_CRITICAL(&mux_);

    if (state_ == State::Pressed) {
        state_ = pressed ? State::Held : State::Idle;
        emitPress = pressed;
    } else if (state_ == State::Release) {
        state_ = pressed ? State::Held : State::Idle;
    }

    portEXIT_CRITICAL(&mux_);

    if (emitPress) {
        callback_(callbackContext_);
    }
}
