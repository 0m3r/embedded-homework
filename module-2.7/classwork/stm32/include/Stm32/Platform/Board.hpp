#pragma once

#include "stm32f4xx_hal.h"

namespace Board {
void init();
TIM_HandleTypeDef& trafficTimer();
TIM_HandleTypeDef& debounceTimer();
}
