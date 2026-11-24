#pragma once

#include <cstdint>

#include "driver/gpio.h"

struct Config {
    static constexpr gpio_num_t RedLedPin = GPIO_NUM_12;
    static constexpr gpio_num_t YellowLedPin = GPIO_NUM_16;
    static constexpr gpio_num_t GreenLedPin = GPIO_NUM_17;
    static constexpr gpio_num_t ModeButtonPin = GPIO_NUM_21;

    static constexpr std::uint32_t DebounceMs = 50;
    static constexpr std::uint32_t PollIntervalMs = 5;
    static constexpr std::uint32_t GreenMs = 5'000;
    static constexpr std::uint32_t GreenFlashHalfPeriodMs = 500;
    static constexpr std::uint32_t GreenFlashSteps = 6;
    static constexpr std::uint32_t YellowMs = 2'000;
    static constexpr std::uint32_t RedMs = 5'000;
    static constexpr std::uint32_t RedYellowMs = 1'000;
    static constexpr std::uint32_t SpecialFlashHalfPeriodMs = 500;
};
