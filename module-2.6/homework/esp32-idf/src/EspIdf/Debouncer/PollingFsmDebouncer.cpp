#include "EspIdf/Debouncer/PollingFsmDebouncer.hpp"

PollingFsmDebouncer::PollingFsmDebouncer(
    std::uint32_t debounceMs)
    : debounceMs_(debounceMs)
{
}

bool PollingFsmDebouncer::update(
    bool pressed,
    std::uint64_t nowMs)
{
    switch (state_) {
    case State::Released:
        if (pressed) {
            state_ = State::DebouncingPress;
            stateSinceMs_ = nowMs;
        }
        break;

    case State::DebouncingPress:
        if (!pressed) {
            state_ = State::Released;
        } else if (nowMs - stateSinceMs_ >= debounceMs_) {
            state_ = State::Pressed;
            ++pressCount_;
            return true;
        }
        break;

    case State::Pressed:
        if (!pressed) {
            state_ = State::DebouncingRelease;
            stateSinceMs_ = nowMs;
        }
        break;

    case State::DebouncingRelease:
        if (pressed) {
            state_ = State::Pressed;
        } else if (nowMs - stateSinceMs_ >= debounceMs_) {
            state_ = State::Released;
        }
        break;
    }

    return false;
}

std::uint32_t PollingFsmDebouncer::pressCount() const
{
    return pressCount_;
}
