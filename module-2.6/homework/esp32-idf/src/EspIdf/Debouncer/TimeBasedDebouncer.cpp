#include "EspIdf/Debouncer/TimeBasedDebouncer.hpp"

TimeBasedDebouncer::TimeBasedDebouncer(std::uint32_t debounceMs)
    : debounceMs_(debounceMs)
{
}

bool TimeBasedDebouncer::onEdge(
    bool pressed,
    std::uint64_t nowMs)
{
    if (!pressed ||
        (hasPress_ && nowMs - lastPressMs_ < debounceMs_)) {
        return false;
    }

    hasPress_ = true;
    lastPressMs_ = nowMs;
    ++pressCount_;
    return true;
}

std::uint32_t TimeBasedDebouncer::pressCount() const
{
    return pressCount_;
}
