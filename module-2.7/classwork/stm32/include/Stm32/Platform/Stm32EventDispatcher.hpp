#pragma once

#include <cstddef>

#include "Application/Event/EventQueue.hpp"
#include "Application/Event/IEventDispatcher.hpp"

class Stm32EventDispatcher final : public IEventDispatcher {
public:
    bool post(const Event& event) override;
    bool registerHandler(IEventHandler& handler) override;
    void dispatchPending() override;
    void wait() override;

private:
    static constexpr std::size_t MaxHandlers = 8;

    void dispatch(const Event& event);

    EventQueue queue_;
    IEventHandler* handlers_[MaxHandlers]{};
    std::size_t handlerCount_ = 0;
};
