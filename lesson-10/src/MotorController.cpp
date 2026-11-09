#include "MotorController.h"

void MotorController::init()
{
    motor_.init();
}

void MotorController::update(uint16_t lightLevel, uint32_t now)
{
    const MotorState requested = requestedState(lightLevel);

    if (requested == motor_.state())
    {
        pending_ = false;
        return;
    }

    if (!pending_)
    {
        pending_ = true;
        pendingSince_ = now;
        return;
    }

    if (now - pendingSince_ >= confirmTimeMs_)
    {
        motor_.set(requested);
        pending_ = false;
    }
}

MotorState MotorController::state() const
{
    return motor_.state();
}

bool MotorController::isPending() const
{
    return pending_;
}

MotorState MotorController::requestedState(uint16_t lightLevel) const
{
    if (lightLevel < darkThreshold_)
    {
        return MotorState::On;
    }

    if (lightLevel > lightThreshold_)
    {
        return MotorState::Off;
    }

    return motor_.state();
}
