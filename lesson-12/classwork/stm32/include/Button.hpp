#pragma once

#include <cstdint>

#include "Clock.hpp"
#include "IntervalTimer.hpp"
#include "stm32f4xx_hal.h"

class Button {
public:
    Button(const Clock& clock, std::uint16_t pin, std::uint32_t debounceMs);

    void init();
    void onInterrupt(std::uint16_t pin);
    bool wasPressed();

private:
    bool debounceElapsed() const;

    const std::uint16_t pin_;
    IntervalTimer debounceTimer_;
    volatile bool pressed_ = false;
};
