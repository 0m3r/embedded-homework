#include "Debouncer/TimeBasedDebouncer.hpp"

TimeBasedDebouncer::TimeBasedDebouncer(const Logger& logger,
                                       std::uint32_t debounceMs)
    : logger_(logger), debounceMs_(debounceMs)
{
}

void TimeBasedDebouncer::debounce(const DebounceInput& input)
{
    if (input.interruptCount == 0) {
        return;
    }

    if (count_ == 0 || input.nowMs - lastAcceptedAtMs_ >= debounceMs_) {
        lastAcceptedAtMs_ = input.nowMs;
        logger_.press("Time-based", ++count_);
    }
}
