#pragma once

#include <cstdint>

#include "Core/Hardware/IButtonInput.hpp"
#include "stm32f4xx_hal.h"

class ButtonInput final : public IButtonInput {
public:
    ButtonInput(
        GPIO_TypeDef* port,
        std::uint16_t pin,
        bool activeHigh);

    bool isPressed() const override;

private:
    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
};
