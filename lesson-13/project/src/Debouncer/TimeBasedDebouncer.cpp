#include "Debouncer/TimeBasedDebouncer.hpp"

TimeBasedDebouncer::TimeBasedDebouncer(const IReporter& reporter,
                                       std::uint32_t debounceMs)
    : reporter_(reporter), debounceMs_(debounceMs)
{
}

void TimeBasedDebouncer::debounce(const DebounceInput& input)
{
    if (input.interruptCount == 0) {
        return;
    }

    if (count_ == 0 || input.nowMs - lastAcceptedAtMs_ >= debounceMs_) {
        lastAcceptedAtMs_ = input.nowMs;
        reporter_.press("Time-based", ++count_);
    }
}
