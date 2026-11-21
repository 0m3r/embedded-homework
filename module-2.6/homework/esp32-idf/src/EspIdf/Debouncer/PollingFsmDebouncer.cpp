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
    case State::Idle:
        if (pressed) {
            state_ = State::Pressed;
            stateSinceMs_ = nowMs;
        }
        break;

    case State::Pressed:
        if (!pressed) {
            state_ = State::Idle;
        } else if (nowMs - stateSinceMs_ >= debounceMs_) {
            state_ = State::Held;
            ++pressCount_;
            return true;
        }
        break;

    case State::Held:
        if (!pressed) {
            state_ = State::Release;
            stateSinceMs_ = nowMs;
        }
        break;

    case State::Release:
        if (pressed) {
            state_ = State::Held;
        } else if (nowMs - stateSinceMs_ >= debounceMs_) {
            state_ = State::Idle;
        }
        break;
    }

    return false;
}

std::uint32_t PollingFsmDebouncer::pressCount() const
{
    return pressCount_;
}
