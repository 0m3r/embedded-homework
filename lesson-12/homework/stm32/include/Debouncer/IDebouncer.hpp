#pragma once

#include <cstdint>

struct DebounceInput {
    std::uint32_t interruptCount;
    bool pressed;
    std::uint32_t nowMs;
};

class IDebouncer {
public:
    virtual ~IDebouncer() = default;
    virtual void debounce(const DebounceInput& input) = 0;
};
