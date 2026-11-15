#include "Clock.hpp"

#include "stm32f4xx_hal.h"

void Clock::update()
{
    nowMs_ = HAL_GetTick();
}

std::uint32_t Clock::now() const
{
    return nowMs_;
}
