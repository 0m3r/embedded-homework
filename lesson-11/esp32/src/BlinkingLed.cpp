#include "BlinkingLed.h"

#include <Arduino.h>

void BlinkingLed::init()
{
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    timer_.reset();
}

void BlinkingLed::update()
{
    if (enabled_ && timer_.ready())
    {
        toggle();
    }
}

void BlinkingLed::on()
{
    if (enabled_)
    {
        return;
    }

    enabled_ = true;
    setState(false);
    timer_.reset();
}

void BlinkingLed::off()
{
    if (!enabled_)
    {
        return;
    }

    enabled_ = false;
    setState(false);
    timer_.reset();
}

void BlinkingLed::toggle()
{
    setState(!state_);
}

void BlinkingLed::setState(bool state)
{
    if (state == state_)
    {
        return;
    }

    state_ = state;
    digitalWrite(pin_, state ? HIGH : LOW);
}
