#pragma once

#include "Application/Event/Event.hpp"
#include "Application/Event/IEventHandler.hpp"

class IEventDispatcher {
public:
    virtual ~IEventDispatcher() = default;

    virtual bool post(const Event& event) = 0;
    virtual bool registerHandler(IEventHandler& handler) = 0;
    virtual void dispatchPending() = 0;
    virtual void wait() = 0;
};
