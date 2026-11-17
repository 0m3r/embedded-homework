#include "PeriodicTimer.hpp"

#include <limits>

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(Handler handler, void* context)
{
    if (handler == nullptr) {
        return false;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();

    const std::uint32_t timerClockHz = HAL_RCC_GetPCLK1Freq() * 2U;

    timer_.Instance = TIM2;
    timer_.Init.Prescaler = timerClockHz / CounterFrequencyHz - 1U;
    timer_.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_.Init.Period = std::numeric_limits<std::uint32_t>::max();
    timer_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer_.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&timer_) != HAL_OK) {
        return false;
    }

    handler_ = handler;
    context_ = context;
    instance_ = this;

    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    return true;
}

bool PeriodicTimer::startOnce(std::uint32_t intervalMs)
{
    const std::uint64_t ticks =
        static_cast<std::uint64_t>(intervalMs) *
        CounterFrequencyHz / 1'000;

    if (ticks == 0 ||
        ticks > std::numeric_limits<std::uint32_t>::max()) {
        return false;
    }

    __HAL_TIM_DISABLE(&timer_);
    __HAL_TIM_DISABLE_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_SET_AUTORELOAD(
        &timer_, static_cast<std::uint32_t>(ticks - 1U));
    __HAL_TIM_SET_COUNTER(&timer_, 0);
    __HAL_TIM_CLEAR_FLAG(&timer_, TIM_FLAG_UPDATE);
    __HAL_TIM_ENABLE_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&timer_);
    return true;
}

void PeriodicTimer::stop()
{
    __HAL_TIM_DISABLE_IT(&timer_, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE(&timer_);
    __HAL_TIM_SET_COUNTER(&timer_, 0);
}

void PeriodicTimer::handleInterrupt()
{
    if (instance_ == nullptr) {
        return;
    }

    auto& timer = *instance_;
    if (__HAL_TIM_GET_FLAG(&timer.timer_, TIM_FLAG_UPDATE) == RESET ||
        __HAL_TIM_GET_IT_SOURCE(&timer.timer_, TIM_IT_UPDATE) == RESET) {
        return;
    }

    __HAL_TIM_CLEAR_IT(&timer.timer_, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE_IT(&timer.timer_, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE(&timer.timer_);

    if (timer.handler_ != nullptr) {
        timer.handler_(timer.context_);
    }
}

extern "C" void TIM2_IRQHandler()
{
    PeriodicTimer::handleInterrupt();
}
