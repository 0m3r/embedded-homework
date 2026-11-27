#pragma once

#include "Application/TrafficLight.hpp"
#include "Hardware/DebugLogger.hpp"
#include "Platform/Stm32EventDispatcher.hpp"
#include "Platform/Stm32EventProducer.hpp"

class Application {
public:
    void init();
    void update();
    void waitForEvent();
    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    Stm32EventDispatcher events_;
    Stm32EventProducer eventProducer_{events_};
    TrafficLight trafficLight_;
    DebugLogger logger_;
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appWaitForEvent();
extern "C" void appButtonInterrupt(std::uint16_t pin);
extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer);
