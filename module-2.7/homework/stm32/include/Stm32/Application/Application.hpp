#pragma once

#include "Application/TrafficLight.hpp"
#include "Hardware/DebugLogger.hpp"

class Application {
public:
    void init();
    void update();
    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    DebugLogger logger_;
    TrafficLight trafficLight_;
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appButtonInterrupt(std::uint16_t pin);
extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer);
