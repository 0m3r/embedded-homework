#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t LedPin = 16;
    static constexpr uint8_t ButtonPin = 17;

    static constexpr uint32_t BlinkIntervalMs = 500;
    static constexpr uint32_t DebounceMs = 40;
    static constexpr uint32_t SerialBaud = 115200;
    static constexpr uint32_t LoopReportIterations = 1000;
};

static_assert(
    Config::LoopReportIterations > 0,
    "LoopReportIterations must be greater than zero"
);
