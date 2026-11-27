#include "Platform/Stm32EventDispatcher.hpp"

#include "stm32f4xx_hal.h"

namespace {
std::uint32_t enterCritical()
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();
    return interruptState;
}

void leaveCritical(std::uint32_t interruptState)
{
    __set_PRIMASK(interruptState);
}
}

bool Stm32EventDispatcher::post(const Event& event)
{
    const auto interruptState = enterCritical();
    const bool posted = queue_.post(event);
    leaveCritical(interruptState);
    return posted;
}

bool Stm32EventDispatcher::registerHandler(IEventHandler& handler)
{
    const auto interruptState = enterCritical();

    for (std::size_t index = 0; index < handlerCount_; ++index) {
        if (handlers_[index] == &handler) {
            leaveCritical(interruptState);
            return true;
        }
    }

    if (handlerCount_ >= MaxHandlers) {
        leaveCritical(interruptState);
        return false;
    }

    handlers_[handlerCount_++] = &handler;
    leaveCritical(interruptState);
    return true;
}

void Stm32EventDispatcher::dispatchPending()
{
    Event event{};

    while (true) {
        const auto interruptState = enterCritical();
        const bool available = queue_.take(event);
        leaveCritical(interruptState);

        if (!available) {
            return;
        }

        dispatch(event);
    }
}

void Stm32EventDispatcher::wait()
{
    const auto interruptState = enterCritical();

    if (queue_.empty()) {
        __WFI();
    }

    leaveCritical(interruptState);
}

void Stm32EventDispatcher::dispatch(const Event& event)
{
    for (std::size_t index = 0; index < handlerCount_; ++index) {
        auto* handler = handlers_[index];
        if (handler->handles(event.type)) {
            handler->handle(event);
        }
    }
}
