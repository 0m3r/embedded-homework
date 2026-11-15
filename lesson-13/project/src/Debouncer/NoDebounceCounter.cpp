#include "Debouncer/NoDebounceCounter.hpp"

NoDebounceCounter::NoDebounceCounter(const IReporter& reporter)
    : reporter_(reporter)
{
}

void NoDebounceCounter::debounce(const DebounceInput& input)
{
    if (input.interruptCount > 0) {
        count_ += input.interruptCount;
        reporter_.press("No debounce", count_);
    }
}
