#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class Led {
public:
    Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeLow);

    void off();
    void on();
    void toggle();

private:
    void write(bool enabled);

    GPIO_TypeDef* port_;
    const std::uint16_t pin_;
    const bool activeLow_;
    bool enabled_ = false;
};
