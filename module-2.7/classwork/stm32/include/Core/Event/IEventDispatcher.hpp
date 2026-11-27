#pragma once

#include "Core/Event/Event.hpp"
#include "Core/Event/IEventHandler.hpp"

class IEventDispatcher {
public:
    virtual ~IEventDispatcher() = default;

    virtual bool post(const Event& event) = 0;
    virtual bool registerHandler(IEventHandler& handler) = 0;
    virtual void dispatchPending() = 0;
    virtual void wait() = 0;
};
