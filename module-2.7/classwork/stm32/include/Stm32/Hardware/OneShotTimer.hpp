#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class OneShotTimer {
public:
    explicit OneShotTimer(TIM_HandleTypeDef& timer);

    void start(std::uint32_t durationMs);

private:
    TIM_HandleTypeDef& timer_;
};
