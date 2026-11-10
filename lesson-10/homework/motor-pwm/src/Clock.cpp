#include "Clock.h"

#include <Arduino.h>

void Clock::update()
{
    nowUs_ = micros();
    nowMs_ = millis();
}

uint32_t Clock::nowMs() const
{
    return nowMs_;
}

uint32_t Clock::nowUs() const
{
    return nowUs_;
}
