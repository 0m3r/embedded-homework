#include "PeriodicTimer.hpp"

#include <limits>

PeriodicTimer* PeriodicTimer::instance_ = nullptr;

bool PeriodicTimer::init(std::uint32_t intervalMs)
{
    if (intervalMs == 0) {
        return false;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();

    const std::uint32_t timerClockHz = HAL_RCC_GetPCLK1Freq() * 2U;
    constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    handle_.Instance = TIM2;
    handle_.Init.Prescaler = timerClockHz / CounterFrequencyHz - 1U;
    handle_.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle_.Init.Period =
        intervalMs * (CounterFrequencyHz / 1'000U) - 1U;
    handle_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle_.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&handle_) != HAL_OK) {
        return false;
    }

    instance_ = this;
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    return true;
}

bool PeriodicTimer::start()
{
    pendingPeriods_ = 0;
    __HAL_TIM_SET_COUNTER(&handle_, 0);
    return HAL_TIM_Base_Start_IT(&handle_) == HAL_OK;
}

void PeriodicTimer::stop()
{
    HAL_TIM_Base_Stop_IT(&handle_);
    __HAL_TIM_SET_COUNTER(&handle_, 0);
    pendingPeriods_ = 0;
}

std::uint32_t PeriodicTimer::getPeriods()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const auto elapsedPeriods = pendingPeriods_;
    pendingPeriods_ = 0;

    if (interruptState == 0U) {
        __enable_irq();
    }
    return elapsedPeriods;
}

void PeriodicTimer::handleInterrupt()
{
    if (instance_ == nullptr) {
        return;
    }

    auto& timer = *instance_;
    if (__HAL_TIM_GET_FLAG(&timer.handle_, TIM_FLAG_UPDATE) == RESET ||
        __HAL_TIM_GET_IT_SOURCE(&timer.handle_, TIM_IT_UPDATE) == RESET) {
        return;
    }

    __HAL_TIM_CLEAR_IT(&timer.handle_, TIM_IT_UPDATE);
    if (timer.pendingPeriods_ < std::numeric_limits<std::uint32_t>::max()) {
        timer.pendingPeriods_ = timer.pendingPeriods_ + 1U;
    }
}

extern "C" void TIM2_IRQHandler()
{
    PeriodicTimer::handleInterrupt();
}
