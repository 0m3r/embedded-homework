#pragma once

#include <cstdint>

#include "Hardware/Led.hpp"
#include "Hardware/OneShotTimer.hpp"

class TrafficLight {
public:
    TrafficLight(
        Led& red,
        Led& yellow,
        Led& green,
        OneShotTimer& timer);

    void init();
    void update();
    void onTimerInterrupt(TIM_HandleTypeDef* timer);
    void toggleSpecialMode();

private:
    enum class State {
        Green,
        GreenFlashingOn,
        GreenFlashingOff,
        Yellow,
        Red,
        RedYellow,
        FlashingYellowOn,
        FlashingYellowOff,
    };

    void next();
    void enter(State state);
    void setLights(bool red, bool yellow, bool green);

    Led& red_;
    Led& yellow_;
    Led& green_;
    OneShotTimer& timer_;
    State state_ = State::Red;
    std::uint32_t greenFlashStep_ = 0;
    bool specialMode_ = false;
};
