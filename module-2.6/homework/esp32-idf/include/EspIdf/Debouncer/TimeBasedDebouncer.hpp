#pragma once

#include <cstdint>

class TimeBasedDebouncer {
public:
    explicit TimeBasedDebouncer(std::uint32_t debounceMs);

    bool onEdge(bool pressed, std::uint64_t nowMs);
    std::uint32_t pressCount() const;

private:
    const std::uint32_t debounceMs_;
    std::uint64_t lastPressMs_ = 0;
    std::uint32_t pressCount_ = 0;
    bool hasPress_ = false;
};
