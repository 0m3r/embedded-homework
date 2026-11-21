#pragma once

#include <cstdint>

class NoDebouncer {
public:
    bool onEdge(bool pressed);
    std::uint32_t pressCount() const;

private:
    std::uint32_t pressCount_ = 0;
};
