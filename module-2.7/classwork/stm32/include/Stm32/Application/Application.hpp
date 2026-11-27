#pragma once

#include "Application/EventDispatcher.hpp"
#include "Application/EventProducer.hpp"
#include "Application/TrafficLight.hpp"
#include "Application/TrafficLightEventHandler.hpp"
#include "Hardware/DebugLogger.hpp"

class Application {
public:
    void init();
    void update();
    void waitForEvent();
    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    EventDispatcher events_;
    EventProducer eventProducer_{events_};
    TrafficLight trafficLight_;
    TrafficLightEventHandler trafficLightEvents_{trafficLight_};
    DebugLogger logger_;
};

extern "C" void appInit();
extern "C" void appUpdate();
extern "C" void appWaitForEvent();
extern "C" void appButtonInterrupt(std::uint16_t pin);
extern "C" void appTimerInterrupt(TIM_HandleTypeDef* timer);
