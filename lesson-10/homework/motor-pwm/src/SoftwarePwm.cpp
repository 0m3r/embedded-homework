#include "SoftwarePwm.h"

#include <Arduino.h>

void SoftwarePwm::init()
{
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    cycleStartedAtUs_ = clock_.nowUs();
}

void SoftwarePwm::setDuty(uint8_t duty)
{
    duty_ = duty;
}

void SoftwarePwm::update()
{
    const uint32_t nowUs = clock_.nowUs();
    const uint32_t elapsedUs = nowUs - cycleStartedAtUs_;
    if (elapsedUs >= periodUs_)
    {
        cycleStartedAtUs_ += (elapsedUs / periodUs_) * periodUs_;
    }

    const uint32_t positionUs = nowUs - cycleStartedAtUs_;
    const uint32_t highTimeUs =
        (periodUs_ * static_cast<uint32_t>(duty_)) / 255U;

    setState(duty_ != 0 && (duty_ == 255 || positionUs < highTimeUs));
}

uint8_t SoftwarePwm::duty() const
{
    return duty_;
}

void SoftwarePwm::setState(bool state)
{
    if (state == state_)
    {
        return;
    }

    state_ = state;
    digitalWrite(pin_, state ? HIGH : LOW);
}
