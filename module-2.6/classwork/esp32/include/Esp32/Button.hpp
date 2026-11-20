#pragma once

#include <Arduino.h>

#include <cstdint>

#include "Esp32/Hardware/OneShotTimer.hpp"

class Button {
public:
    Button(std::uint8_t pin, std::uint32_t debounceMs);

    bool init();
    bool takePress();

private:
    static void ARDUINO_ISR_ATTR gpioInterrupt(void* context);
    static void ARDUINO_ISR_ATTR timerCallback(void* context);
    void ARDUINO_ISR_ATTR onGpioInterrupt();
    void ARDUINO_ISR_ATTR onDebounceTimeout();

    const std::uint8_t pin_;
    const std::uint32_t debounceMs_;

    OneShotTimer timer_;
    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
    volatile bool stablePressed_ = false;
    volatile bool pressPending_ = false;
};
