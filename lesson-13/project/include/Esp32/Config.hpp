#pragma once

#include <cstdint>

struct Config {
    static constexpr std::uint8_t ButtonPin = 21;
    static constexpr std::uint32_t SerialBaud = 115200;
    static constexpr std::uint32_t DebounceMs = 50;
    static constexpr std::uint32_t PollIntervalMs = 5;
};
