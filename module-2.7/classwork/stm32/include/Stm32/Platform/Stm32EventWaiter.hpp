#pragma once

#include "Application/EventDispatcher.hpp"
#include "Application/ICriticalSection.hpp"

class Stm32EventWaiter {
public:
    explicit Stm32EventWaiter(ICriticalSection& criticalSection);

    void wait(const EventDispatcher& dispatcher);

private:
    ICriticalSection& criticalSection_;
};
