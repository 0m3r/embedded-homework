#pragma once

#include <stdint.h>

enum class MotorState : uint8_t
{
    Off,
    On
};

class Motor
{
public:
    explicit constexpr Motor(uint8_t pin)
        : pin_(pin)
    {
    }

    void init();
    void set(MotorState state);
    MotorState state() const;

private:
    const uint8_t pin_;
    MotorState state_ = MotorState::Off;
};
