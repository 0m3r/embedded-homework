#include "Debouncer/NoDebounceCounter.hpp"

NoDebounceCounter::NoDebounceCounter(const DebugLogger& logger,
                                     const char* source)
    : logger_(logger), source_(source)
{
}

void NoDebounceCounter::debounce(const DebounceInput& input)
{
    if (input.interruptCount > 0) {
        count_ += input.interruptCount;
        logger_.press(source_, "No debounce", count_);
    }
}
