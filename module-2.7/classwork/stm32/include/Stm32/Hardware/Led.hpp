#pragma once

#include <cstdint>

#include "Core/Hardware/ILed.hpp"
#include "stm32f4xx_hal.h"

class Led final : public ILed {
public:
    Led(GPIO_TypeDef* port, std::uint16_t pin, bool activeLow);

    void off() override;
    void on() override;
    void toggle() override;

private:
    void write(bool enabled);

    GPIO_TypeDef* port_;
    const std::uint16_t pin_;
    const bool activeLow_;
    bool enabled_ = false;
};
