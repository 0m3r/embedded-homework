#pragma once

#include <cstddef>

#include "Application/EventQueue.hpp"
#include "Application/IEventHandler.hpp"

class EventDispatcher {
public:
    bool post(EventType type);
    bool registerHandler(IEventHandler& handler);
    void dispatchPending();
    void wait();

private:
    static constexpr std::size_t MaxHandlers = 8;

    void dispatch(const Event& event);

    EventQueue queue_;
    IEventHandler* handlers_[MaxHandlers]{};
    std::size_t handlerCount_ = 0;
};
