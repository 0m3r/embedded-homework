#pragma once

#include <Arduino.h>

#include <cstdint>

class PeriodicTimer {
public:
    bool init(std::uint32_t intervalMs);
    bool start();
    void stop();
    std::uint32_t getPeriods();

private:
    static void ARDUINO_ISR_ATTR handleInterrupt();
    static PeriodicTimer* instance_;

    static constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    hw_timer_t* timer_ = nullptr;
    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
    volatile std::uint32_t pendingPeriods_ = 0;
};
