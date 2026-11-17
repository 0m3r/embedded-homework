#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

struct Config {
    static inline GPIO_TypeDef* const FanPort = GPIOC;
    static constexpr std::uint16_t FanPin = GPIO_PIN_15;
    static constexpr bool FanActiveHigh = true;
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
