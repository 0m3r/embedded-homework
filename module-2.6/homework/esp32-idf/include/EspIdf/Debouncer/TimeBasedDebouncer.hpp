#pragma once

#include <cstdint>

class TimeBasedDebouncer {
public:
    explicit TimeBasedDebouncer(std::uint32_t debounceMs)
        : debounceMs_(debounceMs)
    {
    }

    bool onEdge(bool pressed, std::uint64_t nowMs)
    {
        if (!pressed ||
            (hasPress_ && nowMs - lastPressMs_ < debounceMs_)) {
            return false;
        }

        hasPress_ = true;
        lastPressMs_ = nowMs;
        ++pressCount_;
        return true;
    }

    std::uint32_t pressCount() const
    {
        return pressCount_;
    }

private:
    const std::uint32_t debounceMs_;
    std::uint64_t lastPressMs_ = 0;
    std::uint32_t pressCount_ = 0;
    bool hasPress_ = false;
};
