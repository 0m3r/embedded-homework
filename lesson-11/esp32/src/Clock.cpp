#include "Clock.h"

#include <Arduino.h>

void Clock::update()
{
    nowMs_ = millis();
}

uint32_t Clock::nowMs() const
{
    return nowMs_;
}
