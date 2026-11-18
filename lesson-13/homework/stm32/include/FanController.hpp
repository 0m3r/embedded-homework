#pragma once

#include <cstdint>

#include "OneShotTimer.hpp"
#include "stm32f4xx_hal.h"

class FanController {
public:
    enum class Event : std::uint8_t {
        None,
        Started,
        Stopped,
    };

    FanController(GPIO_TypeDef* port,
                  std::uint16_t pin,
                  bool activeHigh,
                  std::uint32_t runDurationMs,
                  std::uint32_t idleDurationMs);

    bool begin();
    Event takeEvent();

private:
    static void timerCallback(void* context);
    void onTimeout();
    void on();
    void off();
    void toggle();
    void write(bool enabled);

    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
    const std::uint32_t runDurationMs_;
    const std::uint32_t idleDurationMs_;

    OneShotTimer timer_;
    volatile Event pendingEvent_ = Event::None;
    volatile bool running_ = false;
};
