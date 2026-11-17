#include "IntervalTimer.hpp"

IntervalTimer::IntervalTimer(const Clock& clock, std::uint32_t intervalMs)
    : clock_(clock), intervalMs_(intervalMs)
{
}

void IntervalTimer::reset()
{
    startedAtMs_ = clock_.now();
}

bool IntervalTimer::elapsed() const
{
    return clock_.now() - startedAtMs_ >= intervalMs_;
}
