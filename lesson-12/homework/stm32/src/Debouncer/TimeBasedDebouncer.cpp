#include "Debouncer/TimeBasedDebouncer.hpp"

TimeBasedDebouncer::TimeBasedDebouncer(const DebugLogger& logger,
                                       const char* source,
                                       std::uint32_t debounceMs)
    : logger_(logger), source_(source), debounceMs_(debounceMs)
{
}

void TimeBasedDebouncer::debounce(const DebounceInput& input)
{
    if (input.interruptCount == 0) {
        return;
    }

    if (count_ == 0 || input.nowMs - lastAcceptedAtMs_ >= debounceMs_) {
        lastAcceptedAtMs_ = input.nowMs;
        logger_.press(source_, "Time-based", ++count_);
    }
}
