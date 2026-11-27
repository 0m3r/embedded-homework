#include "Application/EventDispatcher.hpp"

#include "stm32f4xx_hal.h"

bool EventDispatcher::post(EventType type)
{
    return queue_.post(type);
}

bool EventDispatcher::registerHandler(IEventHandler& handler)
{
    for (std::size_t index = 0; index < handlerCount_; ++index) {
        if (handlers_[index] == &handler) {
            return true;
        }
    }

    if (handlerCount_ >= MaxHandlers) {
        return false;
    }

    handlers_[handlerCount_++] = &handler;
    return true;
}

void EventDispatcher::dispatchPending()
{
    Event event{};
    while (queue_.take(event)) {
        dispatch(event);
    }
}

void EventDispatcher::wait()
{
    __disable_irq();
    if (queue_.empty()) {
        __WFI();
    }
    __enable_irq();
}

void EventDispatcher::dispatch(const Event& event)
{
    for (std::size_t index = 0; index < handlerCount_; ++index) {
        auto* handler = handlers_[index];
        if (handler->handles(event.type)) {
            handler->handle(event);
        }
    }
}
