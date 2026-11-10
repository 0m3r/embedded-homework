#include "SoftwarePwm.h"

#include <Arduino.h>

void SoftwarePwm::init(uint32_t nowUs)
{
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    cycleStartedAtUs_ = nowUs;
}

void SoftwarePwm::setDuty(uint8_t duty)
{
    duty_ = duty;
}

void SoftwarePwm::update(uint32_t nowUs)
{
    const uint32_t elapsedUs = nowUs - cycleStartedAtUs_;
    if (elapsedUs >= periodUs_)
    {
        cycleStartedAtUs_ += (elapsedUs / periodUs_) * periodUs_;
    }

    const uint32_t positionUs = nowUs - cycleStartedAtUs_;
    const uint32_t highTimeUs =
        (periodUs_ * static_cast<uint32_t>(duty_)) / 255U;

    write(duty_ != 0 && (duty_ == 255 || positionUs < highTimeUs));
}

uint8_t SoftwarePwm::duty() const
{
    return duty_;
}

void SoftwarePwm::write(bool high)
{
    if (high == outputHigh_)
    {
        return;
    }

    outputHigh_ = high;
    digitalWrite(pin_, high ? HIGH : LOW);
}
