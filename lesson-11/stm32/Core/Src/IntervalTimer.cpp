#include "IntervalTimer.hpp"

IntervalTimer::IntervalTimer(std::uint32_t intervalMs)
    : intervalMs_(intervalMs)
{
}

bool IntervalTimer::elapsed(std::uint32_t nowMs)
{
    if (nowMs - previousMs_ < intervalMs_) {
        return false;
    }

    previousMs_ = nowMs;
    return true;
}
