#pragma once

#include <Arduino.h>

#include <cstdint>

class PeriodicTimer {
public:
    using Handler = void (*)(void* context);

    bool init(std::uint32_t intervalMs, Handler handler, void* context);
    bool start();
    void stop();

private:
    static void ARDUINO_ISR_ATTR handleInterrupt();
    static PeriodicTimer* instance_;

    static constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    hw_timer_t* timer_ = nullptr;
    Handler handler_ = nullptr;
    void* context_ = nullptr;
};
