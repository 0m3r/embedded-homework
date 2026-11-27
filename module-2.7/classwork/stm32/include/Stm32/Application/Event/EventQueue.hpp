#pragma once

#include <cstddef>
#include <cstdint>

#include "Application/Event/Event.hpp"

class EventQueue {
public:
    bool post(const Event& event);
    bool take(Event& event);
    bool empty() const;
    std::uint32_t dropped() const;

private:
    static constexpr std::size_t Capacity = 16;
    static constexpr std::size_t Mask = Capacity - 1;

    static_assert((Capacity & Mask) == 0, "Event queue capacity must be a power of two");

    Event events_[Capacity]{};
    volatile std::size_t head_ = 0;
    volatile std::size_t tail_ = 0;
    volatile std::uint32_t dropped_ = 0;
};
