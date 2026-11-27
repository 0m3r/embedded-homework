#pragma once

#include <cstdint>

#include "Core/Hardware/IButtonInput.hpp"
#include "Core/Hardware/IOneShotTimer.hpp"

class DebouncedButton {
public:
    using PressCallback = void (*)(void* context);

    DebouncedButton(
        IButtonInput& input,
        IOneShotTimer& timer,
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

    void handleEdge(bool pressed);

    IButtonInput& input_;
    IOneShotTimer& timer_;
    const std::uint32_t debounceMs_;
    PressCallback callback_ = nullptr;
    void* callbackContext_ = nullptr;
    State state_ = State::Idle;
};
