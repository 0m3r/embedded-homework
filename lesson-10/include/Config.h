#pragma once

#include <stdint.h>

struct Config
{
    static constexpr uint8_t LdrPin = 2;
    static constexpr uint8_t MotorPin = 17;

    static constexpr uint16_t DarkThreshold = 1800;
    static constexpr uint16_t LightThreshold = 2200;
    static constexpr uint8_t FilterSize = 8;

    static constexpr uint32_t AdcIntervalMs = 20;
    static constexpr uint32_t PrintIntervalMs = 500;
    static constexpr uint32_t ConfirmTimeMs = 500;
    static constexpr uint32_t SerialBaud = 115200;
};

static_assert(Config::FilterSize > 0, "FilterSize must be greater than zero");
static_assert(
    Config::DarkThreshold < Config::LightThreshold,
    "DarkThreshold must be less than LightThreshold"
);
