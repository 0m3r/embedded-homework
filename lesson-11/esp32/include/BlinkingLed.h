#pragma once

#include <stdint.h>

#include "Clock.h"
#include "IntervalTimer.h"

class BlinkingLed
{
public:
    constexpr BlinkingLed(
        const Clock& clock,
        uint8_t pin,
        uint32_t intervalMs
    )
        : pin_(pin),
          timer_(clock, intervalMs)
    {
    }

    void init();
    void update();
    void on();
    void off();
    void toggle();

private:
    void setState(bool state);

    const uint8_t pin_;
    IntervalTimer timer_;
    bool enabled_ = true;
    bool state_ = false;
};
