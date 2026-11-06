#pragma once

#include <stdint.h>

enum class LedState : uint8_t
{
    Off,
    On
};

class Led
{
public:
    explicit constexpr Led(uint8_t pin)
        : pin_(pin)
    {
    }

    void init();
    void set(LedState state);
    LedState state() const;

private:
    const uint8_t pin_;
    LedState state_ = LedState::Off;
};
