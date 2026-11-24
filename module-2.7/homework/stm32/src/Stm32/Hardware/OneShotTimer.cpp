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

    const auto interruptState = __get_PRIMASK();
    __disable_irq();
    elapsed_ = false;
    if (interruptState == 0U) {
        __enable_irq();
    }

    HAL_TIM_Base_Start_IT(&timer_);
}

void OneShotTimer::onInterrupt(TIM_HandleTypeDef* timer)
{
    if (timer != &timer_) {
        return;
    }

    elapsed_ = true;
}

bool OneShotTimer::takeElapsed()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const bool elapsed = elapsed_;
    elapsed_ = false;

    if (interruptState == 0U) {
        __enable_irq();
    }
    return elapsed;
}
