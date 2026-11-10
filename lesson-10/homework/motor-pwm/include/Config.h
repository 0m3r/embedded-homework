#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t MotorPin = 17;
    static constexpr uint8_t PotentiometerPin = 2;
    static constexpr uint8_t UartTxPin = 4;

    static constexpr uint32_t AdcIntervalMs = 20;
    static constexpr uint32_t PrintIntervalMs = 500;
    static constexpr uint32_t SoftwarePwmPeriodUs = 2000;
    static constexpr uint32_t SerialBaud = 115200;
    static constexpr uint32_t UartBaud = 9600;
    static constexpr uint32_t UartPatternIntervalMs = 500;
};

static_assert(
    Config::SoftwarePwmPeriodUs > 0,
    "Software PWM period must be greater than zero"
);
