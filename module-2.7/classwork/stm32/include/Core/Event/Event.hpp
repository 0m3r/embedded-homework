#pragma once

#include <cstdint>

using EventType = std::uint16_t;

struct Event {
    EventType type;
    std::uint32_t timestampMs;
};
