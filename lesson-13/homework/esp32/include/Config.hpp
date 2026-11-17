#pragma once

#include <cstdint>

struct Config {
    static constexpr std::uint8_t FanPin = 21;
    static constexpr bool FanActiveHigh = true;
    static constexpr std::uint32_t SerialBaud = 115200;

#ifdef FAN_DEBUG_TIMING
    static constexpr std::uint32_t CyclePeriodMs = 20'000;
    static constexpr std::uint32_t RunDurationMs = 5'000;
#else
    static constexpr std::uint32_t CyclePeriodMs = 60 * 60 * 1'000;
    static constexpr std::uint32_t RunDurationMs = 15 * 60 * 1'000;
#endif

    static constexpr std::uint32_t IdleDurationMs =
        CyclePeriodMs - RunDurationMs;

    static_assert(RunDurationMs < CyclePeriodMs);
};
