#include "Stm32/Hardware/OneShotTimer.hpp"

#include <limits>

OneShotTimer* OneShotTimer::timer2_ = nullptr;

OneShotTimer::OneShotTimer(TIM_TypeDef* instance)
{
    timer_.Instance = instance;
}

bool OneShotTimer::init(Callback callback, void* context)
{
    if (callback == nullptr) {
        return false;
    }

    if (timer_.Instance != TIM2) {
        return false;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();
    timer2_ = this;

    const std::uint32_t timerClockHz = HAL_RCC_GetPCLK1Freq() * 2U;

    timer_.Init.Prescaler = timerClockHz / FrequencyHz - 1U;
    timer_.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_.Init.Period = std::numeric_limits<std::uint32_t>::max();
    timer_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer_.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&timer_) != HAL_OK) {
        return false;
    }

    callback_ = callback;
    context_ = context;

    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    return true;
}

bool OneShotTimer::start(std::uint32_t intervalMs)
{
    const std::uint64_t ticks =
        static_cast<std::uint64_t>(intervalMs) * FrequencyHz / 1'000;

    if (ticks == 0 ||
        ticks > std::numeric_limits<std::uint32_t>::max()) {
        return false;
    }

    __HAL_TIM_DISABLE(&timer_);
    __HAL_TIM_SET_AUTORELOAD(
        &timer_, static_cast<std::uint32_t>(ticks - 1U));
    __HAL_TIM_SET_COUNTER(&timer_, 0);
    __HAL_TIM_CLEAR_FLAG(&timer_, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&timer_);
    return true;
}

void OneShotTimer::handleInterrupt(TIM_TypeDef* instance)
{
    if (instance == TIM2 && timer2_ != nullptr) {
        timer2_->handleInterrupt();
    }
}

void OneShotTimer::handleInterrupt()
{
    if (__HAL_TIM_GET_FLAG(&timer_, TIM_FLAG_UPDATE) == RESET ||
        __HAL_TIM_GET_IT_SOURCE(&timer_, TIM_IT_UPDATE) == RESET) {
        return;
    }

    __HAL_TIM_CLEAR_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE(&timer_);
    callback_(context_);
}

extern "C" void TIM2_IRQHandler()
{
    OneShotTimer::handleInterrupt(TIM2);
}
