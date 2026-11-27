#include "Application/EventDispatcher.hpp"

EventDispatcher::EventDispatcher(ICriticalSection& criticalSection)
    : queue_(criticalSection)
{
}

bool EventDispatcher::post(const Event& event)
{
    return queue_.post(event);
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

bool EventDispatcher::hasPendingEvents() const
{
    return !queue_.empty();
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
