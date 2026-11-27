#pragma once

#include <cstdint>

#include "Application/Event/EventDispatcher.hpp"
#include "stm32f4xx_hal.h"

class EventProducer {
public:
    explicit EventProducer(EventDispatcher& dispatcher);

    void onButtonInterrupt(std::uint16_t pin);
    void onTimerInterrupt(TIM_HandleTypeDef* timer);

private:
    EventDispatcher& dispatcher_;
};
