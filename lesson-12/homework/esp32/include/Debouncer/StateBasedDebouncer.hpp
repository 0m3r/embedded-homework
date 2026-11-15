#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Logger.hpp"

class StateBasedDebouncer : public IDebouncer {
public:
    StateBasedDebouncer(const Logger& logger, std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    const Logger& logger_;
    const std::uint32_t debounceMs_;
    std::uint32_t eventStartedAtMs_ = 0;
    std::uint32_t releaseStartedAtMs_ = 0;
    std::uint32_t count_ = 0;
    bool eventPending_ = false;
    bool pressLatched_ = false;
    bool releasePending_ = false;
};
