#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/IReporter.hpp"

class TimeBasedDebouncer : public IDebouncer {
public:
    TimeBasedDebouncer(const IReporter& reporter, std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;

private:
    const IReporter& reporter_;
    const std::uint32_t debounceMs_;
    std::uint32_t lastAcceptedAtMs_ = 0;
    std::uint32_t count_ = 0;
};
