#include "Core/Event/EventQueue.hpp"

bool EventQueue::post(const Event& event)
{
    const auto next = (head_ + 1U) & Mask;
    if (next == tail_) {
        ++dropped_;
        return false;
    }

    events_[head_] = event;
    head_ = next;
    return true;
}

bool EventQueue::take(Event& event)
{
    if (head_ == tail_) {
        return false;
    }

    event = events_[tail_];
    tail_ = (tail_ + 1U) & Mask;
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
