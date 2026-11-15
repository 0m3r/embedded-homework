#pragma once

#include <cstdint>

#include "Debouncer/IDebouncer.hpp"
#include "Logger.hpp"

class NoDebounceCounter : public IDebouncer {
public:
    explicit NoDebounceCounter(const Logger& logger);
    void debounce(const DebounceInput& input) override;

private:
    const Logger& logger_;
    std::uint32_t count_ = 0;
};
