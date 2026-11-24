#pragma once

#include <cstdint>

#include "Hardware/Button.hpp"
#include "Hardware/Led.hpp"
#include "Hardware/OneShotTimer.hpp"
#include "Platform/Board.hpp"

class TrafficLight {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    static constexpr std::uint32_t DebounceMs = 50;

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
    void toggleSpecialMode();
    static void modeButtonPressed(void* context);

    Button modeButton_{
        GPIOB,
        GPIO_PIN_0,
        true,
        Board::debounceTimer(),
        DebounceMs};
    Led red_{GPIOC, GPIO_PIN_13, false};
    Led yellow_{GPIOC, GPIO_PIN_14, false};
    Led green_{GPIOC, GPIO_PIN_15, false};
    OneShotTimer timer_{Board::trafficTimer()};
    State state_ = State::Red;
    std::uint32_t greenFlashStep_ = 0;
    bool specialMode_ = false;
};
