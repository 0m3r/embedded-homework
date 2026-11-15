#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/IReporter.hpp"
#include "Debouncer/NoDebounceCounter.hpp"
#include "Debouncer/PollingDebouncer.hpp"
#include "Debouncer/StateBasedDebouncer.hpp"
#include "Debouncer/TimeBasedDebouncer.hpp"

class DebounceExperiment {
public:
    DebounceExperiment(const IReporter& reporter,
                       std::uint32_t debounceMs,
                       std::uint32_t pollIntervalMs);

    void update(const DebounceInput& input);
    std::uint32_t acceptedCount() const;

private:
    NoDebounceCounter noDebounce_;
    TimeBasedDebouncer timeBased_;
    StateBasedDebouncer stateBased_;
    PollingDebouncer polling_;
    IDebouncer* debouncers_[4]{
        &noDebounce_, &timeBased_, &stateBased_, &polling_};
};
