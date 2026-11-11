#include "IntervalTimer.h"

bool IntervalTimer::ready()
{
    const uint32_t nowMs = clock_.nowMs();
    if (nowMs - previousMs_ < intervalMs_)
    {
        return false;
    }

    previousMs_ = nowMs;
    return true;
}

void IntervalTimer::reset()
{
    previousMs_ = clock_.nowMs();
}
