#pragma once

#include <cstdint>

class ICriticalSection {
public:
    using State = std::uint32_t;

    virtual ~ICriticalSection() = default;

    virtual State enter() = 0;
    virtual void leave(State state) = 0;
};
