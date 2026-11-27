#pragma once

#include <cstdint>

#include "Core/Hardware/IOneShotTimer.hpp"
#include "stm32f4xx_hal.h"

class OneShotTimer final : public IOneShotTimer {
public:
    explicit OneShotTimer(TIM_HandleTypeDef& timer);

    void start(std::uint32_t durationMs) override;

private:
    TIM_HandleTypeDef& timer_;
};
