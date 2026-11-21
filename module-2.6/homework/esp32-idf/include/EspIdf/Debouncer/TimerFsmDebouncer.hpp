#pragma once

#include <cstdint>

#include "EspIdf/Button.hpp"
#include "esp_err.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

class TimerFsmDebouncer {
public:
    using PressCallback = void (*)(void* context);

    TimerFsmDebouncer(const Button& button, std::uint32_t debounceMs);

    esp_err_t init(PressCallback callback, void* context);
    void onEdge(bool pressed);
    std::uint32_t pressCount() const;

private:
    enum class State {
        Released,
        DebouncingPress,
        Pressed,
        DebouncingRelease
    };

    static void timerCallback(void* context);
    void onTimeout();
    void restartTimer();

    const Button& button_;
    const std::uint64_t debounceUs_;
    esp_timer_handle_t timer_ = nullptr;
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    State state_ = State::Released;
    std::uint32_t pressCount_ = 0;
    mutable portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
};
