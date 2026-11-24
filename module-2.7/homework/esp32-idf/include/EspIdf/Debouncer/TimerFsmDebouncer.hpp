#pragma once

#include <cstdint>

#include "EspIdf/Button.hpp"
#include "EspIdf/OneShotTimer.hpp"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

class TimerFsmDebouncer {
public:
    using PressCallback = void (*)(void* context);

    TimerFsmDebouncer(const Button& button, std::uint32_t debounceMs);

    esp_err_t init(PressCallback callback, void* context);
    void onEdge(bool pressed);

private:
    enum class State {
        Idle,
        Pressed,
        Held,
        Release,
    };

    static void timerCallback(void* context);
    void onTimeout();

    const Button& button_;
    const std::uint32_t debounceMs_;
    OneShotTimer timer_{"mode_button"};
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    State state_ = State::Idle;
    mutable portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
};
