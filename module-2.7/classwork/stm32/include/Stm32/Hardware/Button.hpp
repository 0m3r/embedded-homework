#pragma once

#include <cstdint>

#include "Hardware/OneShotTimer.hpp"
#include "stm32f4xx_hal.h"

class Button {
public:
    using PressCallback = void (*)(void* context);

    Button(
        GPIO_TypeDef* port,
        std::uint16_t pin,
        bool activeHigh,
        TIM_HandleTypeDef& timer,
        std::uint32_t debounceMs);

    void init(PressCallback callback, void* context);
    void onEdge();
    void onTimeout();

private:
    enum class State {
        Idle,
        Pressed,
        Held,
        Release,
    };

    bool isPressed() const;
    void handleEdge(bool pressed);

    GPIO_TypeDef* const port_;
    const std::uint16_t pin_;
    const bool activeHigh_;
    const std::uint32_t debounceMs_;
    OneShotTimer timer_;
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    State state_ = State::Idle;
};
