#pragma once

#include <stdint.h>

#include "Clock.h"
#include "IntervalTimer.h"

class Debouncer
{
public:
    constexpr Debouncer(const Clock& clock, uint32_t intervalMs)
        : timer_(clock, intervalMs)
    {
    }

    void init(bool value);
    bool update(bool value);
    bool state() const;

private:
    IntervalTimer timer_;
    bool input_ = false;
    bool state_ = false;
};
