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
                  std::uint32_t cyclePeriodSeconds,
                  std::uint32_t runDurationSeconds);

    void begin();
    void ARDUINO_ISR_ATTR update();
    Events takeEvents();

private:
    void ARDUINO_ISR_ATTR write(bool enabled);

    const std::uint8_t pin_;
    const bool activeHigh_;
    const std::uint32_t cyclePeriodSeconds_;
    const std::uint32_t runDurationSeconds_;

    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
    volatile std::uint32_t elapsedSeconds_ = 0;
    volatile std::uint32_t pendingStarts_ = 0;
    volatile std::uint32_t pendingStops_ = 0;
    volatile bool running_ = false;
};
