#pragma once

#include <stdint.h>

class Relay
{
public:
    explicit constexpr Relay(uint8_t controlPin)
        : controlPin_(controlPin)
    {
    }

    void init();
    void on();
    void off();
    void toggle();

private:
    const uint8_t controlPin_;
    bool enabled_ = false;
};
