#pragma once

#include "Core/Event/Event.hpp"

namespace TrafficEvent {
constexpr EventType ButtonEdge = 0;
constexpr EventType DebounceTimeout = 1;
constexpr EventType TrafficTimeout = 2;
}
