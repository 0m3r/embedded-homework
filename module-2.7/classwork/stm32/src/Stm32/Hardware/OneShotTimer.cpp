#include "Hardware/OneShotTimer.hpp"

OneShotTimer::OneShotTimer(TIM_HandleTypeDef& timer)
    : timer_(timer)
{
}

void OneShotTimer::start(std::uint32_t durationMs)
{
    HAL_TIM_Base_Stop_IT(&timer_);
    __HAL_TIM_SET_COUNTER(&timer_, 0);
    __HAL_TIM_SET_AUTORELOAD(&timer_, durationMs * 10U - 1U);
    __HAL_TIM_CLEAR_FLAG(&timer_, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&timer_);
}
