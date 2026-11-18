#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class OneShotTimer {
public:
    using Callback = void (*)(void* context);

    bool init(Callback callback, void* context);
    bool start(std::uint32_t intervalMs);

    static void interruptHandler();

private:
    static OneShotTimer* instance_;
    static constexpr std::uint32_t FrequencyHz = 1'000'000;

    TIM_HandleTypeDef timer_{};
    Callback callback_ = nullptr;
    void* context_ = nullptr;
};
