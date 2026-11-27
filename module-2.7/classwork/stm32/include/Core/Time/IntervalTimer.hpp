#pragma once

#include <cstdint>

#include "Core/Time/IClock.hpp"

class IntervalTimer {
public:
    IntervalTimer(
        const IClock& clock,
        std::uint32_t intervalMs);

    void reset();
    bool elapsed() const;
    bool take();

private:
    const IClock& clock_;
    const std::uint32_t intervalMs_;
    std::uint32_t startedAtMs_ = 0;
};
