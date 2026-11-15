#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Debouncer/IReporter.hpp"

class NoDebounceCounter : public IDebouncer {
public:
    explicit NoDebounceCounter(const IReporter& reporter);
    void debounce(const DebounceInput& input) override;

private:
    const IReporter& reporter_;
    std::uint32_t count_ = 0;
};
