#pragma once

#include <cstdint>

#include "Core/Hardware/ILed.hpp"
#include "Core/Hardware/IOneShotTimer.hpp"

class LedFlasher {
public:
    static constexpr std::uint32_t Continuous = 0;

    explicit LedFlasher(IOneShotTimer& timer);

    void start(
        ILed& led,
        std::uint32_t halfPeriodMs,
        std::uint32_t halfPeriods = Continuous);
    bool onTimeout();
    void stop();

private:
    IOneShotTimer& timer_;
    ILed* led_ = nullptr;
    std::uint32_t halfPeriodMs_ = 0;
    std::uint32_t remainingHalfPeriods_ = 0;
    bool continuous_ = false;
};
