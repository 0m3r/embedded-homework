#include "Debouncer/NoDebounceCounter.hpp"

NoDebounceCounter::NoDebounceCounter(const Logger& logger)
    : logger_(logger)
{
}

void NoDebounceCounter::debounce(const DebounceInput& input)
{
    for (std::uint32_t i = 0; i < input.interruptCount; ++i) {
        logger_.press("No debounce", ++count_);
    }
}
