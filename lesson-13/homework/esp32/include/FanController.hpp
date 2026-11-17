#pragma once

#include <Arduino.h>

#include <cstdint>

class FanController {
public:
    struct Events {
        std::uint32_t starts;
        std::uint32_t stops;
    };

    FanController(std::uint8_t pin,
                  bool activeHigh,
                  std::uint32_t runDurationMs,
                  std::uint32_t idleDurationMs);

    void begin();
    std::uint32_t ARDUINO_ISR_ATTR onTimeout();
    Events takeEvents();

private:
    void ARDUINO_ISR_ATTR on();
    void ARDUINO_ISR_ATTR off();
    void ARDUINO_ISR_ATTR toggle();
    void ARDUINO_ISR_ATTR write(bool enabled);

    const std::uint8_t pin_;
    const bool activeHigh_;
    const std::uint32_t runDurationMs_;
    const std::uint32_t idleDurationMs_;

    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
    volatile std::uint32_t pendingStarts_ = 0;
    volatile std::uint32_t pendingStops_ = 0;
    volatile bool running_ = false;
};
