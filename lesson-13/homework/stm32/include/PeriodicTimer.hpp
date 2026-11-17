#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class PeriodicTimer {
public:
    using Handler = void (*)(void* context);

    bool init(Handler handler, void* context);
    bool startOnce(std::uint32_t intervalMs);
    void stop();

    static void handleInterrupt();

private:
    static PeriodicTimer* instance_;
    static constexpr std::uint32_t CounterFrequencyHz = 1'000'000;

    TIM_HandleTypeDef timer_{};
    Handler handler_ = nullptr;
    void* context_ = nullptr;
};
