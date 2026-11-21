#pragma once

#include <cstdint>

class PollingFsmDebouncer {
public:
    explicit PollingFsmDebouncer(std::uint32_t debounceMs)
        : debounceMs_(debounceMs)
    {
    }

    bool update(bool pressed, std::uint64_t nowMs)
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

    std::uint32_t pressCount() const
    {
        return pressCount_;
    }

private:
    enum class State {
        Released,
        DebouncingPress,
        Pressed,
        DebouncingRelease
    };

    const std::uint32_t debounceMs_;
    State state_ = State::Released;
    std::uint64_t stateSinceMs_ = 0;
    std::uint32_t pressCount_ = 0;
};
