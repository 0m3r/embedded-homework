#pragma once

#include <cstdint>

#include "Hardware/Button.hpp"
#include "Hardware/OneShotTimer.hpp"

class TimerFsmDebouncer {
public:
    using PressCallback = void (*)(void* context);

    TimerFsmDebouncer(
        const Button& button,
        OneShotTimer& timer,
        std::uint32_t debounceMs);

    void init(PressCallback callback, void* context);
    void onEdge(bool pressed);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);
    void update();

private:
    enum class State {
        Idle,
        Pressed,
        Held,
        Release,
    };

    const Button& button_;
    OneShotTimer& timer_;
    const std::uint32_t debounceMs_;
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    State state_ = State::Idle;
};
