#pragma once

#include <stdint.h>

#include "Led.h"
#include "LedMode.h"

class LedController
{
public:
    constexpr LedController(uint8_t ledPin, uint32_t blinkIntervalMs)
        : led_(ledPin),
          blinkIntervalMs_(blinkIntervalMs)
    {
    }

    void init(uint32_t now);
    void update(uint32_t now);
    void next(uint32_t now);
    LedMode mode() const;

private:
    void applyMode();

    Led led_;
    const uint32_t blinkIntervalMs_;
    LedMode mode_ = LedMode::Blink;
    uint32_t lastBlinkAt_ = 0;
};
