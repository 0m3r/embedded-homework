#pragma once

#include "Application/ICriticalSection.hpp"

class Stm32CriticalSection final : public ICriticalSection {
public:
    State enter() override;
    void leave(State state) override;
};
