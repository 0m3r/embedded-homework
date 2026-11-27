#pragma once

#include <cstddef>

#include "Application/Event/EventQueue.hpp"
#include "Application/Event/IEventHandler.hpp"

class EventDispatcher {
public:
    explicit EventDispatcher(ICriticalSection& criticalSection);

    bool post(const Event& event);
    bool registerHandler(IEventHandler& handler);
    void dispatchPending();
    bool hasPendingEvents() const;

private:
    static constexpr std::size_t MaxHandlers = 8;

    void dispatch(const Event& event);

    EventQueue queue_;
    IEventHandler* handlers_[MaxHandlers]{};
    std::size_t handlerCount_ = 0;
};
