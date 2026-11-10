#include "Timer.h"

bool Timer::ready()
{
    const uint32_t nowMs = clock_.nowMs();
    if (nowMs - previousMs_ < intervalMs_)
    {
        return false;
    }

    previousMs_ = nowMs;
    return true;
}

void Timer::reset()
{
    previousMs_ = clock_.nowMs();
}
