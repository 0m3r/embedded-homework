#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class FanController {
public:
    struct Events {
        std::uint32_t starts;
        std::uint32_t stops;
    };

    FanController(GPIO_TypeDef* port,
                  std::uint16_t pin,
                  bool activeHigh,
                  std::uint32_t runDurationMs,
                  std::uint32_t idleDurationMs);

    void begin();
    std::uint32_t onTimeout();
    Events takeEvents();

private:
    void on();
    void off();
    void toggle();
    void write(bool enabled);

    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
    const std::uint32_t runDurationMs_;
    const std::uint32_t idleDurationMs_;

    volatile std::uint32_t pendingStarts_ = 0;
    volatile std::uint32_t pendingStops_ = 0;
    volatile bool running_ = false;
};
