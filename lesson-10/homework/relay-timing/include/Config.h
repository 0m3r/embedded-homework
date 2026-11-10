#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t RelayControlPin = 16;
    static constexpr uint8_t RelayFeedbackPin = 18;

    static constexpr uint8_t RelayMeasurementCount = 10;
    static constexpr uint32_t RelayDebounceUs = 10000;
    static constexpr uint32_t RelaySettleUs = 500000;
    static constexpr uint32_t RelayTimeoutUs = 2000000;
    static constexpr uint32_t RepeatIntervalMs = 60000;
    static constexpr uint32_t SerialBaud = 115200;
};

static_assert(
    Config::RelayMeasurementCount >= 10,
    "At least 10 relay measurements are required"
);
static_assert(
    Config::RelayDebounceUs < Config::RelayTimeoutUs,
    "Relay debounce must be shorter than timeout"
);
