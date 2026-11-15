#pragma once

#include <cstdint>

#include "Hardware/DebugLogger.hpp"
#include "Debouncer/IDebouncer.hpp"

class TimeBasedDebouncer : public IDebouncer {
public:
    TimeBasedDebouncer(const DebugLogger& logger,
                       const char* source,
                       std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    const DebugLogger& logger_;
    const char* const source_;
    const std::uint32_t debounceMs_;
    std::uint32_t lastAcceptedAtMs_ = 0;
    std::uint32_t count_ = 0;
};
