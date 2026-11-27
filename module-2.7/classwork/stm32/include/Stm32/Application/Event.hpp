#pragma once

#include <cstdint>

enum class EventType : std::uint8_t {
    ButtonEdge,
    DebounceTimeout,
    TrafficTimeout,
};

struct Event {
    EventType type;
    std::uint32_t timestampMs;
};
