#pragma once

#include <stdint.h>

#include "Clock.h"

class IntervalTimer
{
public:
    constexpr IntervalTimer(const Clock& clock, uint32_t intervalMs)
        : clock_(clock),
          intervalMs_(intervalMs)
    {
    }

    bool ready();
    void reset();

private:
    const Clock& clock_;
    const uint32_t intervalMs_;
    uint32_t previousMs_ = 0;
};
