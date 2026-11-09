#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t RelayControlPin = 16;
    static constexpr uint8_t RelayContactPin = 18;
    static constexpr uint8_t MotorPin = 17;
    static constexpr uint8_t PotentiometerPin = 2;
    static constexpr uint8_t UartTxPin = 4;

    static constexpr uint8_t RelayMeasurementCount = 10;
    static constexpr uint32_t RelayDebounceUs = 10000;
    static constexpr uint32_t RelaySettleUs = 500000;
    static constexpr uint32_t RelayTimeoutUs = 2000000;

    static constexpr uint32_t AdcIntervalMs = 20;
    static constexpr uint32_t PrintIntervalMs = 500;
    static constexpr uint32_t SoftwarePwmPeriodUs = 2000;

    static constexpr uint32_t SerialBaud = 115200;
    static constexpr uint32_t UartBaud = 9600;
    static constexpr uint32_t UartPatternIntervalMs = 500;
};

static_assert(
    Config::RelayMeasurementCount >= 10,
    "At least 10 relay measurements are required"
);
static_assert(
    Config::RelayDebounceUs < Config::RelayTimeoutUs,
    "Relay debounce must be shorter than timeout"
);
static_assert(
    Config::SoftwarePwmPeriodUs > 0,
    "Software PWM period must be greater than zero"
);
