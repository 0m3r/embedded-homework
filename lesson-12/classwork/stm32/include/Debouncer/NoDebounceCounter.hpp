#pragma once

#include <cstdint>

#include "Hardware/DebugLogger.hpp"
#include "Debouncer/IDebouncer.hpp"

class NoDebounceCounter : public IDebouncer {
public:
    NoDebounceCounter(const DebugLogger& logger, const char* source);
    void debounce(const DebounceInput& input) override;

private:
    const DebugLogger& logger_;
    const char* const source_;
    std::uint32_t count_ = 0;
};
