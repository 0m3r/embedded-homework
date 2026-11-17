#pragma once

#include <cstdint>

#include "Clock.hpp"

class IntervalTimer {
public:
    IntervalTimer(const Clock& clock, std::uint32_t intervalMs);

    void reset();
    bool elapsed() const;

private:
    const Clock& clock_;
    const std::uint32_t intervalMs_;
    volatile std::uint32_t startedAtMs_ = 0;
};
