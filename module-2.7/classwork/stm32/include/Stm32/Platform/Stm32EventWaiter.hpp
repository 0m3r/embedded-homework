#pragma once

#include "Application/Event/EventDispatcher.hpp"
#include "Application/Event/ICriticalSection.hpp"

class Stm32EventWaiter {
public:
    explicit Stm32EventWaiter(ICriticalSection& criticalSection);

    void wait(const EventDispatcher& dispatcher);

private:
    ICriticalSection& criticalSection_;
};
