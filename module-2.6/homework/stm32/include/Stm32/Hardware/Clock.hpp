#pragma once

#include <cstdint>

class Clock {
public:
    void update();
    std::uint32_t now() const;

private:
    volatile std::uint32_t nowMs_ = 0;
};
