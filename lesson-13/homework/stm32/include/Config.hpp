#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

struct Config {
    static inline GPIO_TypeDef* const FanPort = GPIOC;
    static constexpr std::uint16_t FanPin = GPIO_PIN_15;
    static constexpr bool FanActiveHigh = true;

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
