#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class Led {
public:
    Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeHigh);

    void init();
    void toggle();
    bool isOn() const;

private:
    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
    bool on_ = false;
};
