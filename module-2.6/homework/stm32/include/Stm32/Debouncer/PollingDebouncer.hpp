#pragma once

#include <cstdint>

#include "Hardware/DebugLogger.hpp"
#include "Debouncer/IDebouncer.hpp"

class PollingDebouncer : public IDebouncer {
public:
    PollingDebouncer(const DebugLogger& logger,
                     const char* source,
                     std::uint32_t pollIntervalMs,
                     std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    enum class State {
        Idle,
        Pressed,
        Held,
        Release,
    };

    const DebugLogger& logger_;
    const char* const source_;
    const std::uint32_t pollIntervalMs_;
    const std::uint32_t debounceMs_;
    std::uint32_t lastPollAtMs_ = 0;
    std::uint32_t transitionStartedAtMs_ = 0;
    std::uint32_t count_ = 0;
    State state_ = State::Idle;
};
