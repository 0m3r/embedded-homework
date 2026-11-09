#include "Motor.h"

#include <Arduino.h>

void Motor::init()
{
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    state_ = MotorState::Off;
}

void Motor::set(MotorState state)
{
    state_ = state;
    digitalWrite(pin_, state_ == MotorState::On ? HIGH : LOW);
}

MotorState Motor::state() const
{
    return state_;
}
