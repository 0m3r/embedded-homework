#include "Debouncer/DebounceExperiment.hpp"

DebounceExperiment::DebounceExperiment(const IReporter& reporter,
                                       std::uint32_t debounceMs,
                                       std::uint32_t pollIntervalMs)
    : noDebounce_(reporter),
      timeBased_(reporter, debounceMs),
      stateBased_(reporter, debounceMs),
      polling_(reporter, pollIntervalMs, debounceMs)
{
}

void DebounceExperiment::update(const DebounceInput& input)
{
    for (auto* debouncer : debouncers_) {
        debouncer->debounce(input);
    }
}

std::uint32_t DebounceExperiment::acceptedCount() const
{
    return stateBased_.count();
}
