#pragma once

#include <Arduino.h>

#include <cstdint>

class OneShotTimer {
public:
    using Callback = void (*)(void* context);

    bool init(Callback callback, void* context);
    bool ARDUINO_ISR_ATTR start(std::uint32_t intervalMs);

private:
    static constexpr std::uint32_t FrequencyHz = 1'000'000;

    hw_timer_t* timer_ = nullptr;
};
