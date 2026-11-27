#pragma once

#include <cstdint>

class IOneShotTimer {
public:
    virtual ~IOneShotTimer() = default;

    virtual void start(std::uint32_t durationMs) = 0;
};
