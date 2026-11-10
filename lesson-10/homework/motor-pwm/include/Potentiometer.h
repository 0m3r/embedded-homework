#pragma once

#include <stdint.h>

#include "Clock.h"
#include "Timer.h"

class Potentiometer
{
public:
    constexpr Potentiometer(
        const Clock& clock,
        uint8_t pin,
        uint32_t sampleIntervalMs
    )
        : pin_(pin),
          timer_(clock, sampleIntervalMs)
    {
    }

    void init();
    bool update();
    uint16_t value() const;
    uint8_t duty() const;

private:
    const uint8_t pin_;
    Timer timer_;
    uint16_t value_ = 0;
};
