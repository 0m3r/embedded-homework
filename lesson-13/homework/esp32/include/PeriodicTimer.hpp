#pragma once

#include <Arduino.h>

#include <cstdint>

class PeriodicTimer {
public:
    using Handler = void (*)(void* context);

    bool init(Handler handler, void* context);
    bool ARDUINO_ISR_ATTR startOnce(std::uint32_t intervalMs);
    void stop();

private:
    static void ARDUINO_ISR_ATTR handleInterrupt();
    static PeriodicTimer* instance_;

    static constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    hw_timer_t* timer_ = nullptr;
    Handler handler_ = nullptr;
    void* context_ = nullptr;
    bool started_ = false;
};
