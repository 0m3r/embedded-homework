#pragma once

#include <cstdint>

#include "driver/gpio.h"

struct Config {
    static constexpr gpio_num_t ButtonNoRcPin = GPIO_NUM_21;
    static constexpr gpio_num_t ButtonRcPin = GPIO_NUM_18;
    static constexpr gpio_num_t LedNoRcPin = GPIO_NUM_12;
    static constexpr gpio_num_t LedRcPin = GPIO_NUM_16;
    static constexpr std::uint32_t DebounceMs = 50;
    static constexpr std::uint32_t PollIntervalMs = 5;
};
