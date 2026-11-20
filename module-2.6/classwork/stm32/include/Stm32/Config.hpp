#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

struct Config {
    static constexpr std::uint32_t DebounceMs = 50;
};
