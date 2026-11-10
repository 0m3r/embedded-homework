#include "Potentiometer.h"

#include <Arduino.h>

void Potentiometer::init()
{
    pinMode(pin_, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(pin_, ADC_11db);
}

bool Potentiometer::update(uint32_t nowMs)
{
    if (nowMs - lastSampleAtMs_ < sampleIntervalMs_)
    {
        return false;
    }

    lastSampleAtMs_ = nowMs;
    value_ = analogRead(pin_);
    return true;
}

uint16_t Potentiometer::value() const
{
    return value_;
}

uint8_t Potentiometer::duty() const
{
    return static_cast<uint8_t>(
        (static_cast<uint32_t>(value_) * 255U) / 4095U
    );
}
