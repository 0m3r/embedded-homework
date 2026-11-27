#pragma once

#include <cstdint>

#include "Application/TrafficEvents.hpp"
#include "Core/Event/IEventDispatcher.hpp"
#include "stm32f4xx_hal.h"

class Stm32EventProducer {
public:
    explicit Stm32EventProducer(IEventDispatcher& dispatcher);

    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    IEventDispatcher& dispatcher_;
};
