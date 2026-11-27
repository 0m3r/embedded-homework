#include "Application/EventQueue.hpp"

#include "stm32f4xx_hal.h"

bool EventQueue::post(EventType type)
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    const auto next = (head_ + 1U) & Mask;
    if (next == tail_) {
        ++dropped_;
        __set_PRIMASK(interruptState);
        return false;
    }

    events_[head_] = Event{type, HAL_GetTick()};
    head_ = next;

    __set_PRIMASK(interruptState);
    return true;
}

bool EventQueue::take(Event& event)
{
    const auto interruptState = __get_PRIMASK();
    __disable_irq();

    if (head_ == tail_) {
        __set_PRIMASK(interruptState);
        return false;
    }

    event = events_[tail_];
    tail_ = (tail_ + 1U) & Mask;

    __set_PRIMASK(interruptState);
    return true;
}

bool EventQueue::empty() const
{
    return head_ == tail_;
}

std::uint32_t EventQueue::dropped() const
{
    return dropped_;
}
