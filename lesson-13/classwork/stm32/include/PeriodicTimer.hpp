#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class PeriodicTimer {
public:
    bool init(std::uint32_t intervalMs);
    bool start();
    void stop();
    std::uint32_t getPeriods();

    static void handleInterrupt();

private:
    static PeriodicTimer* instance_;

    TIM_HandleTypeDef handle_{};
    volatile std::uint32_t pendingPeriods_ = 0;
};
