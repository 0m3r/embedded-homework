#pragma once

#include <stdint.h>

enum class LedMode : uint8_t
{
    Blink,
    AlwaysOn,
    AlwaysOff
};

constexpr LedMode nextMode(LedMode mode)
{
    switch (mode)
    {
        case LedMode::Blink:
            return LedMode::AlwaysOn;

        case LedMode::AlwaysOn:
            return LedMode::AlwaysOff;

        case LedMode::AlwaysOff:
            return LedMode::Blink;
    }

    return LedMode::AlwaysOff;
}

constexpr const char* modeName(LedMode mode)
{
    switch (mode)
    {
        case LedMode::Blink:
            return "BLINK";

        case LedMode::AlwaysOn:
            return "ON";

        case LedMode::AlwaysOff:
            return "OFF";
    }

    return "UNKNOWN";
}
