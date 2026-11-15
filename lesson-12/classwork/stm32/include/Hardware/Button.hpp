#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class Button {
public:
    Button(GPIO_TypeDef* port, std::uint16_t pin, bool activeHigh);

    void init();
    void onInterrupt(std::uint16_t pin);
    bool isPressed() const;
    std::uint32_t takeInterrupts();

private:
    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
    volatile std::uint32_t pendingInterrupts_ = 0;
};
