#pragma once

#include <stdint.h>

#include "Clock.h"
#include "Debouncer.h"

class Button
{
public:
    constexpr Button(
        const Clock& clock,
        uint8_t pin,
        uint32_t debounceMs
    )
        : pin_(pin),
          debouncer_(clock, debounceMs)
    {
    }

    void init();
    bool update();

private:
    bool read() const;

    const uint8_t pin_;
    Debouncer debouncer_;
};
