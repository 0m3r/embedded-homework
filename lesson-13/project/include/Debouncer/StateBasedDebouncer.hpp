#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/IReporter.hpp"

class StateBasedDebouncer : public IDebouncer {
public:
    StateBasedDebouncer(const IReporter& reporter, std::uint32_t debounceMs);
    void debounce(const DebounceInput& input) override;
    std::uint32_t count() const;

private:
    const IReporter& reporter_;
    const std::uint32_t debounceMs_;
    std::uint32_t eventStartedAtMs_ = 0;
    std::uint32_t releaseStartedAtMs_ = 0;
    std::uint32_t count_ = 0;
    bool eventPending_ = false;
    bool pressLatched_ = false;
    bool releasePending_ = false;
};
