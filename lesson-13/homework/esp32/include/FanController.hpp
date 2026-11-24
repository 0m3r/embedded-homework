#pragma once

#include <Arduino.h>

#include <cstdint>

#include "OneShotTimer.hpp"

class FanController {
public:
    enum class Event : std::uint8_t {
        None,
        Started,
        Stopped,
        TimerError,
    };

    FanController(std::uint8_t pin,
                  bool activeHigh,
                  std::uint32_t runDurationMs,
                  std::uint32_t idleDurationMs);

    bool begin();
    Event takeEvent();

private:
    static void ARDUINO_ISR_ATTR timerCallback(void* context);
    void ARDUINO_ISR_ATTR onTimeout();
    void ARDUINO_ISR_ATTR on();
    void ARDUINO_ISR_ATTR off();
    void ARDUINO_ISR_ATTR toggle();
    void ARDUINO_ISR_ATTR write(bool enabled);

    const std::uint8_t pin_;
    const bool activeHigh_;
    const std::uint32_t runDurationMs_;
    const std::uint32_t idleDurationMs_;

    OneShotTimer timer_;
    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
    volatile Event pendingEvent_ = Event::None;
    volatile bool running_ = false;
};
