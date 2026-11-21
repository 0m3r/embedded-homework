#pragma once

#include <cstdint>

class PollingFsmDebouncer {
public:
    explicit PollingFsmDebouncer(std::uint32_t debounceMs);

    bool update(bool pressed, std::uint64_t nowMs);
    std::uint32_t pressCount() const;

private:
    enum class State {
        Idle,
        Pressed,
        Held,
        Release
    };

    const std::uint32_t debounceMs_;
    State state_ = State::Idle;
    std::uint64_t stateSinceMs_ = 0;
    std::uint32_t pressCount_ = 0;
};
