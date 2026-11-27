#include "Core/Time/IntervalTimer.hpp"

IntervalTimer::IntervalTimer(
    const IClock& clock,
    std::uint32_t intervalMs)
    : clock_(clock),
      intervalMs_(intervalMs)
{
}

void IntervalTimer::reset()
{
    startedAtMs_ = clock_.nowMs();
}

bool IntervalTimer::elapsed() const
{
    return clock_.nowMs() - startedAtMs_ >= intervalMs_;
}

bool IntervalTimer::take()
{
    if (!elapsed()) {
        return false;
    }

    startedAtMs_ = clock_.nowMs();
    return true;
}
