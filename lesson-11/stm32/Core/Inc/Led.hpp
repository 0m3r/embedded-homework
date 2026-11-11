#pragma once

#include <cstdint>

#include "main.h"

class Led {
public:
    Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeLow);

    void off();
    void on();
    void toggle();

private:
    void write(bool enabled);

    GPIO_TypeDef* port_;
    std::uint16_t pin_;
    bool activeLow_;
    bool enabled_ = false;
};
