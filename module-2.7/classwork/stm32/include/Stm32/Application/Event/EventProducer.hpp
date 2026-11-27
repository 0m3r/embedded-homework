#pragma once

#include <cstdint>

#include "Application/Event/IEventDispatcher.hpp"
#include "stm32f4xx_hal.h"

class EventProducer {
public:
    explicit EventProducer(IEventDispatcher& dispatcher);

    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    IEventDispatcher& dispatcher_;
};
