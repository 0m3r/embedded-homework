#include "Application/EventQueue.hpp"

EventQueue::EventQueue(ICriticalSection& criticalSection)
    : criticalSection_(criticalSection)
{
}

bool EventQueue::post(const Event& event)
{
    const auto interruptState = criticalSection_.enter();

    const auto next = (head_ + 1U) & Mask;
    if (next == tail_) {
        ++dropped_;
        criticalSection_.leave(interruptState);
        return false;
    }

    events_[head_] = event;
    head_ = next;

    criticalSection_.leave(interruptState);
    return true;
}

bool EventQueue::take(Event& event)
{
    const auto interruptState = criticalSection_.enter();

    if (head_ == tail_) {
        criticalSection_.leave(interruptState);
        return false;
    }

    event = events_[tail_];
    tail_ = (tail_ + 1U) & Mask;

    criticalSection_.leave(interruptState);
    return true;
}

bool EventQueue::empty() const
{
    const auto interruptState = criticalSection_.enter();
    const bool isEmpty = head_ == tail_;
    criticalSection_.leave(interruptState);
    return isEmpty;
}

std::uint32_t EventQueue::dropped() const
{
    const auto interruptState = criticalSection_.enter();
    const auto dropped = dropped_;
    criticalSection_.leave(interruptState);
    return dropped;
}
