#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Logger.hpp"

class PollingDebouncer : public IDebouncer {
public:
    PollingDebouncer(const Logger& logger,
                     std::uint32_t pollIntervalMs,
                     std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    enum class State {
        Released,
        DebouncingPress,
        Pressed,
        DebouncingRelease,
    };

    const Logger& logger_;
    const std::uint32_t pollIntervalMs_;
    const std::uint32_t debounceMs_;
    std::uint32_t lastPollAtMs_ = 0;
    std::uint32_t transitionStartedAtMs_ = 0;
    std::uint32_t count_ = 0;
    State state_ = State::Released;
};
