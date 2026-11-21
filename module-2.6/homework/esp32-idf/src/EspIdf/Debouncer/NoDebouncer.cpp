#include "EspIdf/Debouncer/NoDebouncer.hpp"

bool NoDebouncer::onEdge(bool pressed)
{
    if (pressed) {
        ++pressCount_;
    }
    return pressed;
}

std::uint32_t NoDebouncer::pressCount() const
{
    return pressCount_;
}
