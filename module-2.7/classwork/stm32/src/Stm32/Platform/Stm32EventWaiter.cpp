#include "Platform/Stm32EventWaiter.hpp"

#include "stm32f4xx_hal.h"

Stm32EventWaiter::Stm32EventWaiter(ICriticalSection& criticalSection)
    : criticalSection_(criticalSection)
{
}

void Stm32EventWaiter::wait(const EventDispatcher& dispatcher)
{
    const auto interruptState = criticalSection_.enter();

    if (!dispatcher.hasPendingEvents()) {
        __WFI();
    }

    criticalSection_.leave(interruptState);
}
