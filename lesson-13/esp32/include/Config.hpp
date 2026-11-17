#pragma once

#include <cstdint>

struct Config {
    static constexpr std::uint8_t FanPin = 21;
    static constexpr bool FanActiveHigh = true;
    static constexpr std::uint32_t SerialBaud = 115200;
    static constexpr std::uint32_t TimerTickMs = 1'000;

#ifdef FAN_DEBUG_TIMING
    static constexpr std::uint32_t CyclePeriodSeconds = 20;
    static constexpr std::uint32_t RunDurationSeconds = 5;
#else
    static constexpr std::uint32_t CyclePeriodSeconds = 60 * 60;
    static constexpr std::uint32_t RunDurationSeconds = 15 * 60;
#endif

    static_assert(RunDurationSeconds < CyclePeriodSeconds);
};
