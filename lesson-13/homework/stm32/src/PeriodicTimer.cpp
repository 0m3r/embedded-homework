#include "PeriodicTimer.hpp"

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(std::uint32_t intervalMs,
                         Handler handler,
                         void* context)
{
    if (intervalMs == 0 || handler == nullptr) {
        return false;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();

    const std::uint32_t timerClockHz = HAL_RCC_GetPCLK1Freq() * 2U;
    constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    timer_.Instance = TIM2;
    timer_.Init.Prescaler = timerClockHz / CounterFrequencyHz - 1U;
    timer_.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_.Init.Period =
        intervalMs * (CounterFrequencyHz / 1'000U) - 1U;
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

bool PeriodicTimer::start()
{
    __HAL_TIM_SET_COUNTER(&timer_, 0);
    return HAL_TIM_Base_Start_IT(&timer_) == HAL_OK;
}

void PeriodicTimer::stop()
{
    HAL_TIM_Base_Stop_IT(&timer_);
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
    if (timer.handler_ != nullptr) {
        timer.handler_(timer.context_);
    }
}

extern "C" void TIM2_IRQHandler()
{
    PeriodicTimer::handleInterrupt();
}
