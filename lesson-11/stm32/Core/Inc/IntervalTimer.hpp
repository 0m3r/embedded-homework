#pragma once

#include <cstdint>

class IntervalTimer {
public:
    explicit IntervalTimer(std::uint32_t intervalMs);

    bool elapsed(std::uint32_t nowMs);

private:
    std::uint32_t intervalMs_;
    std::uint32_t previousMs_ = 0;
};
