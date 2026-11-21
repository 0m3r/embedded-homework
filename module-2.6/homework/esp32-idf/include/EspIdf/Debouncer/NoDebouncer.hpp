#pragma once

#include <cstdint>

class NoDebouncer {
public:
    bool onEdge(bool pressed)
    {
        if (pressed) {
            ++pressCount_;
        }
        return pressed;
    }

    std::uint32_t pressCount() const
    {
        return pressCount_;
    }

private:
    std::uint32_t pressCount_ = 0;
};
