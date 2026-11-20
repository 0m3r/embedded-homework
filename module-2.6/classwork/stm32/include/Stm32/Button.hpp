#pragma once

#include <cstdint>

#include "Stm32/Hardware/OneShotTimer.hpp"
#include "stm32f4xx_hal.h"

class Button {
public:
    Button(GPIO_TypeDef* port,
           std::uint16_t pin,
           bool pressedHigh,
           std::uint32_t pull);
    Button(GPIO_TypeDef* port,
           std::uint16_t pin,
           bool pressedHigh,
           std::uint32_t pull,
           TIM_TypeDef* timer,
           std::uint32_t debounceMs);

    bool init();
    void onInterrupt(std::uint16_t pin);
    bool takePress();

private:
    static void timerCallback(void* context);
    void onDebounceTimeout();

    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool pressedHigh_;
    const std::uint32_t pull_;
    const std::uint32_t debounceMs_;
    const IRQn_Type interrupt_;

    OneShotTimer timer_;
    const bool softwareDebounce_;
    volatile bool pressPending_ = false;
};
