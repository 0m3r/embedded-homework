#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class OneShotTimer {
public:
    explicit OneShotTimer(TIM_HandleTypeDef& timer);

    void start(std::uint32_t durationMs);
    void onInterrupt(TIM_HandleTypeDef* timer);
    bool takeElapsed();

private:
    TIM_HandleTypeDef& timer_;
    volatile bool elapsed_ = false;
};
