#pragma once

#include <cstdint>

#include "Hardware/Led.hpp"
#include "Hardware/OneShotTimer.hpp"

class LedFlasher {
public:
    static constexpr std::uint32_t Continuous = 0;

    explicit LedFlasher(OneShotTimer& timer);

    void start(
        Led& led,
        std::uint32_t halfPeriodMs,
        std::uint32_t halfPeriods = Continuous);
    bool onTimeout();
    void stop();

private:
    OneShotTimer& timer_;
    Led* led_ = nullptr;
    std::uint32_t halfPeriodMs_ = 0;
    std::uint32_t remainingHalfPeriods_ = 0;
    bool continuous_ = false;
};
