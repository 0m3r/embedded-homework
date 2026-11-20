#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class OneShotTimer {
public:
    using Callback = void (*)(void* context);

    explicit OneShotTimer(TIM_TypeDef* instance = nullptr);

    bool init(Callback callback, void* context);
    bool start(std::uint32_t intervalMs);

    static void handleInterrupt(TIM_TypeDef* instance);

private:
    static constexpr std::uint32_t FrequencyHz = 1'000'000;
    static OneShotTimer* timer2_;

    void handleInterrupt();

    TIM_HandleTypeDef timer_{};
    Callback callback_ = nullptr;
    void* context_ = nullptr;
};
