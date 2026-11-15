#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Logger.hpp"

class TimeBasedDebouncer : public IDebouncer {
public:
    TimeBasedDebouncer(const Logger& logger, std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    const Logger& logger_;
    const std::uint32_t debounceMs_;
    std::uint32_t lastAcceptedAtMs_ = 0;
    std::uint32_t count_ = 0;
};
