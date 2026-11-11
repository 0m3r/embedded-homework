#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t RedLedPin = 15;
    static constexpr uint8_t GreenLedPin = 16;
    static constexpr uint8_t BlueLedPin = 17;
    static constexpr uint8_t ButtonPin = 21;
    static constexpr uint32_t RedIntervalMs = 250;
    static constexpr uint32_t GreenIntervalMs = 500;
    static constexpr uint32_t BlueIntervalMs = 1000;
    static constexpr uint32_t ButtonDebounceMs = 30;
    static constexpr uint32_t SerialBaud = 115200;
};
