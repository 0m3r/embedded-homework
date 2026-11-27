#include "Platform/Stm32CriticalSection.hpp"

#include "stm32f4xx_hal.h"

ICriticalSection::State Stm32CriticalSection::enter()
{
    const auto state = __get_PRIMASK();
    __disable_irq();
    return state;
}

void Stm32CriticalSection::leave(State state)
{
    __set_PRIMASK(state);
}
