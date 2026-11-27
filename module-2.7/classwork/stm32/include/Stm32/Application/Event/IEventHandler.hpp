#pragma once

#include "Application/Event/Event.hpp"

class IEventHandler {
public:
    virtual ~IEventHandler() = default;

    virtual bool handles(EventType type) const = 0;
    virtual void handle(const Event& event) = 0;
};
