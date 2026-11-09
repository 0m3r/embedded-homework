#include "Ldr.h"

#include <Arduino.h>

void Ldr::init()
{
    pinMode(pin_, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(pin_, ADC_11db);
}

bool Ldr::update(uint32_t now)
{
    if (now - lastSampleAt_ < sampleIntervalMs_)
    {
        return false;
    }

    lastSampleAt_ = now;
    raw_ = analogRead(pin_);
    filtered_ = filter(raw_);
    return true;
}

uint16_t Ldr::raw() const
{
    return raw_;
}

uint16_t Ldr::filtered() const
{
    return filtered_;
}

uint16_t Ldr::filter(uint16_t value)
{
    sum_ -= readings_[index_];
    readings_[index_] = value;
    sum_ += value;

    index_ = (index_ + 1) % Config::FilterSize;

    if (count_ < Config::FilterSize)
    {
        ++count_;
    }

    return static_cast<uint16_t>(sum_ / count_);
}
