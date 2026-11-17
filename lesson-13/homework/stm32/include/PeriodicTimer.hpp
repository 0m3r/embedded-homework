#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"

class PeriodicTimer {
public:
    using Handler = void (*)(void* context);

    bool init(std::uint32_t intervalMs, Handler handler, void* context);
    bool start();
    void stop();

    static void handleInterrupt();

private:
    static PeriodicTimer* instance_;

    TIM_HandleTypeDef timer_{};
    Handler handler_ = nullptr;
    void* context_ = nullptr;
};
