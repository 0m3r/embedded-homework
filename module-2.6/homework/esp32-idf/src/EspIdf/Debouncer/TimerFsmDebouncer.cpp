#include "EspIdf/Debouncer/TimerFsmDebouncer.hpp"

TimerFsmDebouncer::TimerFsmDebouncer(
    const Button& button,
    std::uint32_t debounceMs)
    : button_(button),
      debounceUs_(static_cast<std::uint64_t>(debounceMs) * 1'000)
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

    esp_timer_create_args_t args = {};
    args.callback = timerCallback;
    args.arg = this;
    args.name = "timer_fsm";
    return esp_timer_create(&args, &timer_);
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
        restartTimer();
    }
}

std::uint32_t TimerFsmDebouncer::pressCount() const
{
    portENTER_CRITICAL(&mux_);
    const std::uint32_t count = pressCount_;
    portEXIT_CRITICAL(&mux_);
    return count;
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
        if (pressed) {
            ++pressCount_;
            emitPress = true;
        }
    } else if (state_ == State::Release) {
        state_ = pressed ? State::Held : State::Idle;
    }

    portEXIT_CRITICAL(&mux_);

    if (emitPress) {
        callback_(callbackContext_);
    }
}

void TimerFsmDebouncer::restartTimer()
{
    const esp_err_t result = esp_timer_stop(timer_);
    if (result == ESP_OK || result == ESP_ERR_INVALID_STATE) {
        esp_timer_start_once(timer_, debounceUs_);
    }
}
