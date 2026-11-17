#pragma once

#include <Arduino.h>

#include <cstdint>

class Button {
public:
    explicit Button(std::uint8_t pin);

    void begin();
    bool takePress();

private:
    static void ARDUINO_ISR_ATTR interruptHandler(void* context);
    void ARDUINO_ISR_ATTR onInterrupt();

    const std::uint8_t pin_;
    volatile bool pendingPress_ = false;
};
