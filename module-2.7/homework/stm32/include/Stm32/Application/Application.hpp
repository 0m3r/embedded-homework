#pragma once

#include "Application/TrafficLight.hpp"
#include "Debouncer/TimerFsmDebouncer.hpp"
#include "Hardware/Button.hpp"
#include "Hardware/DebugLogger.hpp"
#include "Hardware/Led.hpp"
#include "Hardware/OneShotTimer.hpp"
#include "Platform/Board.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    static constexpr std::uint32_t DebounceMs = 50;

    static void modeButtonPressed(void* context);

    DebugLogger logger_;
    Button modeButton_{GPIOB, GPIO_PIN_0, true};
    Led red_{GPIOC, GPIO_PIN_13, true};
    Led yellow_{GPIOC, GPIO_PIN_14, false};
    Led green_{GPIOC, GPIO_PIN_15, false};
    OneShotTimer trafficTimer_{Board::trafficTimer()};
    OneShotTimer debounceTimer_{Board::debounceTimer()};
    TimerFsmDebouncer modeButtonDebouncer_{
        modeButton_,
        debounceTimer_,
        DebounceMs};
    TrafficLight trafficLight_{
        red_,
        yellow_,
        green_,
        trafficTimer_};
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer);
