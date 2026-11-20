#pragma once

#include <cstdint>

#include "driver/gpio.h"

struct Config {
    static constexpr gpio_num_t ButtonPin = GPIO_NUM_21;
    static constexpr gpio_num_t LedPin = GPIO_NUM_15;
    static constexpr std::uint32_t DebounceMs = 50;
};
